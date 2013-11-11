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
#define CONNECTOR_TYPE_DISPLAY 0x24

struct Connector;
struct Connection;
struct Gate;
struct Output;
struct Switch;
struct Power;
struct Display;

struct Connector {
  int type;
  union types {
    struct Power * p;
    struct Switch * s;
    struct Output * o;
    struct Gate * g;
    struct Display * d;
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

struct Display {
  char name[500];
  int checks;
  int value;
};

struct Input {
  struct Power powersource;
  struct Switch switches[500];
  struct Output outputs[500];
  struct Gate gates[500];
  struct Display displays[500];
  int num_switches;
  int num_outputs;
  int num_gates;
  int num_displays;
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
      if (gate.inputs == gate.check && gate.check > 1) {
        return 1;
      }
      break;
    case GATE_TYPE_OR:
      if (gate.inputs != 0) {
        return 1;
      }
      break;
    case GATE_TYPE_XOR: 
      if (gate.check > 1 && gate.inputs != gate.check && gate.inputs > 0 && gate.inputs == 1) {
        return 1;
      }
      break;
  }
  return 0;
}

int time = 0;

int run(struct Connector input, int pow, int gate_check);

int run(struct Connector input, int pow, int gate_check) {
  time = time + 1;
  int i, val = 0;
  struct Connection * conn;
  int set = 0;

  int ploris = 1;
  switch (input.type) {
    case CONNECTOR_TYPE_GATE:


      input.conn.g->inputs += pow;
      if (input.conn.g->check != 0)
        ploris = 0;
      if (gate_check == 1)
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
    case CONNECTOR_TYPE_DISPLAY:
      input.conn.d->value += pow * (2^input.conn.d->checks);
      if (gate_check == 1)
        input.conn.d->checks++;
      set = 0;
      break;

    default:
      return 1;
  }
  if (set == 1) {
    for (i = 0; i < conn->num_outputs; i++) {

      run(conn->connectors[i], val, ploris);
    }
  } else {
  }
  return 0;
 
}



int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("You need to pass a file name as an argument. Exiting... \n");
    return 0;
  }
  get_config(argv[1]);
  struct Connector tes;
  tes.type = CONNECTOR_TYPE_POWER;
  tes.conn.p = &(lo.powersource);
  

  int i;
  
  run(tes, 1, 1);
  for (i = 0; i < lo.num_outputs; i++) {
      if (lo.outputs[i].value != 0) {
        printf("Output %s on\n", lo.outputs[i].name);
      }
    }
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
    for (i = 0; i < lo.num_switches; i++) {
		  if (strcmp(lo.switches[i].name, input_c) == 0) {
			  if (strcmp(comp, "on") == 0) {
				  lo.switches[i].value = 1;
			  } else if (strcmp(comp, "off") == 0) {
			   	lo.switches[i].value = 0;
			  }
		  }
	  }

    for (i = 0; i < lo.num_outputs; i++) {
      lo.outputs[i].value = 0;
    }

    run(tes, 1, 1);

    for (i = 0; i < lo.num_outputs; i++) {
      if (lo.outputs[i].value != 0) {
        printf("Output %s on\n", lo.outputs[i].name);
      }
    }
    for (i = 0; i < lo.num_displays; i++) {
      printf("Register %s is %d\n", lo.displays[i].name, lo.displays[i].value);
      lo.displays[i].value = 0;
      lo.displays[i].checks = 0;
    }
  }
  return 1;
}
