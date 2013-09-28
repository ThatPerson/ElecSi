#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define GATE_TYPE_AND 0x10
#define GATE_TYPE_OR 0x11
#define GATE_TYPE_XOR 0x12

#define CONNECTOR_TYPE_POWER 0x20
#define CONNECTOR_TYPE_SWITCH 0x21
#define CONNECTOR_TYPE_OUTPUT 0x22
#define CONNECTOR_TYPE_GATE 0x23

struct Connector;
struct Connection;
struct Gate;
struct Output;
struct Switch;
struct Power;

struct Connector {
  int type;
  union types {
    struct Power * p;
    struct Switch * s;
    struct Output * o;
    struct Gate * g;
  } conn;
};

struct Connection {
  struct Connector connectors[512];
  
  int num_outputs;
};

struct Gate {
  struct Connection connections;
  int inputs; // Changes depending on forward current. Ie, if you have below set to 'AND', it will only pass if inputs = 2;
  int gate_type; //use a GATE_TYPE
  char name[400];
  int checks; //number of times hit.
};

struct Output {
  int value;
  char name[400];
  struct Connection connections;
};

struct Switch {
  int value;
  char name[400];
  struct Connection connections;
};

struct Power {
  int on;
  struct Connection connections;
};


/*
 * Do not dwell on the past. It is not changable.
 * Do not dwell on the future. It is incontrolable.
 * Dwell on the current, the present. What you do 
 * now can revoke the mistakes of the past, and 
 * allow you to see the future.
 */

int logic_gate(struct Gate gate) {
  printf("%#x, %d\n", gate.gate_type, gate.inputs);
  switch (gate.gate_type) {
    case GATE_TYPE_AND:
      if (gate.inputs == 2) {
        return 1;
      }
      break;
    case GATE_TYPE_OR:
      if (gate.inputs != 0) {
        return 1;
      }
      break;
    case GATE_TYPE_XOR: 
      printf("%d %d\n", gate.inputs, gate.checks);
      if (gate.inputs == 1 && gate.inputs != gate.checks) {
        return 1;
      }
      break;
  }
  return 0;
}


int run(struct Connector input, int run);

int run(struct Connector input, int print) {
  int i, runf;
  struct Connection connections;
  switch (input.type) {
    case CONNECTOR_TYPE_GATE:
      if (print == 1) {
        if (logic_gate(*input.conn.g) == 1) {
          printf("Gate %s on\n", input.conn.g->name);
        }
      } else {
        input.conn.g->inputs++;
        input.conn.g->checks--;
        if (input.conn.g->checks != 0) {
          return 0;
        }
      }
      connections = input.conn.g->connections;
      if (logic_gate(*input.conn.g) == 1) {
        runf = 1;
        //printf("Gate %s on\n", input.conn.g->name);
      }
      break;
    case CONNECTOR_TYPE_OUTPUT:
      if (print == 1) {
        printf("Output %s on\n", input.conn.o->name);
      }
      runf = 1;
      connections = input.conn.o->connections;
      break;
    case CONNECTOR_TYPE_POWER:
      runf = 1;
      connections = input.conn.p->connections;
      break;
    case CONNECTOR_TYPE_SWITCH:
      if (input.conn.s->value == 1) {
        runf = 1;
        connections = input.conn.s->connections;
      }
      break;
  }

  if (runf == 1) {
    for (i = 0; i < connections.num_outputs; i++) {
      run(connections.connectors[i], print);
    }
  }
  return 0;
 
}

int main(void) {
  struct Power input;
  struct Switch switc;
  struct Switch lio;
  struct Output test;
  struct Gate gate;
  gate.gate_type = GATE_TYPE_AND;
  
  input.on = 1;
  switc.value = 0;
  strcpy(switc.name, "Switch");
  strcpy(test.name, "Output");
  input.connections.connectors[0].type = CONNECTOR_TYPE_SWITCH;
  input.connections.connectors[0].conn.s = &switc;
  input.connections.connectors[1].type = CONNECTOR_TYPE_SWITCH;
  input.connections.connectors[1].conn.s = &lio;
  switc.connections.connectors[0].type = CONNECTOR_TYPE_GATE;
  switc.connections.connectors[0].conn.g = &gate;
  lio.connections.connectors[0].type = CONNECTOR_TYPE_GATE;
  lio.connections.connectors[0].conn.g = &gate;
  gate.checks = 0;
  gate.connections.connectors[0].type = CONNECTOR_TYPE_OUTPUT;
  gate.connections.connectors[0].conn.o = &test;
  gate.connections.num_outputs = 1;
  gate.checks = 2;
  switc.connections.num_outputs = 1;
  input.connections.num_outputs = 2;
  lio.connections.num_outputs = 1;
  struct Connector tes;
  tes.type = CONNECTOR_TYPE_POWER;
  tes.conn.p = &input;
  run(tes, 0);
  char input_c[500];
  while (strcmp(input_c, "exit") != 0) {
    printf("> ");
    scanf("%499s", input_c);
    gate.inputs = 0;
    gate.checks = 3;
    if (strcmp(input_c, "on1") == 0) {
      switc.value = 1;
    } else if (strcmp(input_c, "off1") == 0) {
      switc.value = 0;
    } else if (strcmp(input_c, "on2") == 0) {
      lio.value = 1;
    } else if (strcmp(input_c, "off2") == 0) {
      lio.value = 0;
    }
    run(tes, 0);
    run(tes, 1);
  }
  return 1;
}
