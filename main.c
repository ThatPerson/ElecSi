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
  struct Connection connections;
};

struct Input {
  struct Power powersource;
  struct Switch switches[500];
  struct Output outputs[500];
  struct Gate gates[500];
  int num_switches;
  int num_outputs;
  int num_gates;
};

struct Input lo;


#include "parse.c"

/*
 * Do not dwell on the past. It is not changable.
 * Do not dwell on the future. It is incontrolable.
 * Dwell on the current, the present. What you do 
 * now can revoke the mistakes of the past, and 
 * allow you to change the future.
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
      if (gate.check > 1 && gate.inputs != gate.check && gate.inputs > 0) {
        return 1;
      }
      break;
  }
  return 0;
}

int time = 0;

int run(struct Connector input, int pow);

int run(struct Connector input, int pow) {
  time = time + 1;
  int i, val = 0;
  struct Connection * conn;
  int set = 0;
  switch (input.type) {
    case CONNECTOR_TYPE_GATE:


      input.conn.g->inputs += pow;
      input.conn.g->check ++;

      conn = &(input.conn.g->connections);
//       printf("%d - %d\n", logic_gate(*input.conn.g), input.conn.g->inputs);
//       if (logic_gate(*input.conn.g) == 1) {
//         val = pow;
//       }
      val = logic_gate(*input.conn.g);
      set = 1;
      break;
    case CONNECTOR_TYPE_OUTPUT:

      if (input.conn.o->value == 0) {
        input.conn.o->value = pow;
      }
      conn = &(input.conn.o->connections);
      val = pow;
      set = 1;
      break;
    case CONNECTOR_TYPE_POWER:

      conn = &(input.conn.p->connections);
      val = pow;
      set = 1;
      break;
    case CONNECTOR_TYPE_SWITCH:
      val = 0;
      if (input.conn.s->value == 1) {
        set = 1;
        
        val = pow;
      }
      conn = &(input.conn.s->connections);
      set = 1;
      break;
    default:
      return 1;
  }
  if (set == 1) {
    for (i = 0; i < conn->num_outputs; i++) {
      run(conn->connectors[i], val);
    }
  } else {
  }
  return 0;
 
}



int main(void) {
  get_config();
  struct Connector tes;
  tes.type = CONNECTOR_TYPE_POWER;
  tes.conn.p = &(lo.powersource);
  

  int i;
  
  run(tes, 1);
  char input_c[500], comp[500];
  while (strcmp(input_c, "exit") != 0) {
    //g.check = 0;
        printf("> ");
    scanf("%499s", input_c);
    //g.inputs = 0;
    scanf("%499s", comp);

    for (i = 0; i < lo.num_gates; i++) {
      lo.gates[i].check = 0;
      lo.gates[i].inputs = 0;
    }
    lo.switches[0].value = 1;
    for (i = 0; i < lo.num_switches; i++) {
		  if (strcmp(lo.switches[i].name, input_c) == 0) {
			  if (strcmp(comp, "on") == 0) {
				  lo.switches[i].value = 1;
			  } else {
			   	lo.switches[i].value = 0;
			  }
		  }
	  }

    for (i = 0; i < lo.num_outputs; i++) {
      lo.outputs[i].value = 0;
    }

    run(tes, 1);

    for (i = 0; i < lo.num_outputs; i++) {
      if (lo.outputs[i].value != 0) {
        printf("Output %s on\n", lo.outputs[i].name);
      }
    }

  }
  return 1;
}
