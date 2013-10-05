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
  int check;
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
  char name[400];
  struct Connection connections;
};

struct Test {
  struct Power p[5];
  struct Gate g[5];
  struct Output o[5];
  struct Switch s[5];
  int num_power;
  int num_gate;
  int num_output;
  int num_switch;
};

#include "parse.c"

/*
 * Do not dwell on the past. It is not changable.
 * Do not dwell on the future. It is incontrolable.
 * Dwell on the current, the present. What you do 
 * now can revoke the mistakes of the past, and 
 * allow you to see the future.
 */

int logic_gate(struct Gate gate) {
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
      if (gate.inputs == 1 && gate.inputs != gate.check) {
        return 1;
      }
      break;
  }
  return 0;
}

int run(struct Connector input, int pow);

int run(struct Connector input, int pow) {
  int i, val = 0;
  struct Connection * conn;
  printf("%d J\n", input.type);
  switch (input.type) {
    case CONNECTOR_TYPE_GATE:
      printf("GATE");
      input.conn.g->inputs += pow;
      input.conn.g->check ++;
      conn = &input.conn.g->connections;
//       printf("%d - %d\n", logic_gate(*input.conn.g), input.conn.g->inputs);
//       if (logic_gate(*input.conn.g) == 1) {
//         val = pow;
//       }
      val = logic_gate(*input.conn.g);
      break;
    case CONNECTOR_TYPE_OUTPUT:
      printf("OUTPUT");
      printf("%d\n", &(input.conn.o->value));

      if (input.conn.o->value == 0) {
        input.conn.o->value = pow;
      }
      conn = &input.conn.o->connections;
      val = pow;
      break;
    case CONNECTOR_TYPE_POWER:
      printf("POWER");
      conn = &input.conn.p->connections;
      val = pow;
      break;
    case CONNECTOR_TYPE_SWITCH:
      printf("SWITCH");
      if (input.conn.s->value == 1) {
        conn = &input.conn.s->connections;
        val = pow;
      }
      break;
    default: printf("No connector"); break;
  }
  printf("%d\n", conn->num_outputs);

  for (i = 0; i < conn->num_outputs; i++) {
    run(conn->connectors[i], val);
  }
  return 0;
 
}

int main(void) {
  printf("HELLO");
  char argv[4][400];
  strcpy(argv[1], "test.sim");
  printf("HEllo");
  struct Test t;
  struct Connector c;
  printf("Hello\n");
  printf("%s\n", argv[1]);
  
  t = get_config(argv[1]);
  c.type = CONNECTOR_TYPE_POWER;
  c.conn.p = &t.p[0];
  run(c, 1);
  /*char input_c[500];
  while (strcmp(input_c, "exit") != 0) {
    g.check = 0;
    printf("> ");
    scanf("%499s", input_c);
    g.inputs = 0;
    if (strcmp(input_c, "on1") == 0) {
      switc.value = 1;
    } else if (strcmp(input_c, "off1") == 0) {
      switc.value = 0;
    } else if (strcmp(input_c, "on2") == 0) {
      lio.value = 1;
    } else if (strcmp(input_c, "off2") == 0) {
      lio.value = 0;
    }
    test.value = 0;
    run(tes, 1);
    if (test.value == 1) {
      printf("OUTPUT on\n");
    }
    
  }*/
  return 1;
}
