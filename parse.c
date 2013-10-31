#include <stdio.h>
#include <stdlib.h>

// Right. We have a struct called Input. In Input we have one power source, and connectors with that. Then connectors to those. And so on.

char * substr(char * orig, int start, int length) {
	char * ret = (char *)malloc (sizeof (char) * length);
	strncpy (ret, orig+start, length);
	ret[length] = '\0';
	return ret;
}

struct LocationObject {
	/*
	 * Lazy hackish way of doing it, just store metadata. I was wanting to use pointers, but considering how ElecSi is dragging on quite long, I will try to do it as quick as possibly.
	 */
	int type;
	int array_num;
};

struct LocationObject get_connector(char name[500]) {
	struct LocationObject connect;
	connect.type = -1;
	int ql;
	for (ql = 0; ql < lo.num_outputs; ql++) {
		printf("%s =/= %s ?\n", lo.outputs[ql].name, name);
		if (strcmp(lo.outputs[ql].name, name) == 0) {
			connect.type = CONNECTOR_TYPE_OUTPUT;
			connect.array_num = ql;
		}
	}
	for (ql = 0; ql < lo.num_gates; ql++) {
		printf("%s =/= %s ?\n", lo.gates[ql].name, name);

		if (strcmp(lo.gates[ql].name, name) == 0) {
			connect.type = CONNECTOR_TYPE_GATE;
			connect.array_num = ql;	
		}
	}
	for (ql = 0; ql < lo.num_switches; ql++) {
		printf("%s =/= %s ?\n", lo.switches[ql].name, name);
		if (strcmp(lo.switches[ql].name, name) == 0) {
			connect.type = CONNECTOR_TYPE_SWITCH;
			connect.array_num = ql;	
		}
	}
	return connect;
}

int get_config(void) {

	char filename[500];
	strcpy(filename, "test.sim");


	FILE * file = fopen(filename, "r");
	if (file != NULL) {
		char line[128];
		lo.num_switches = 0;
		lo.num_outputs = 0;
		lo.num_gates = 0;
		while (fgets(line, sizeof(line), file) != NULL) {
			switch (line[0]) {
				case 'S':
					strcpy(lo.switches[lo.num_switches].name, substr(line, 2, strlen(line)-3));
					printf("%s (%d) ((%s) %d %d)\n", lo.switches[lo.num_switches].name, lo.num_switches, line, 2, strlen(line)-2);
					lo.num_switches++;
					break;
				case 'O':
					strcpy(lo.outputs[lo.num_outputs].name, substr(line, 2, strlen(line)-3));
					printf("%s (%d) ((%s) %d %d)\n", lo.outputs[lo.num_switches].name, lo.num_outputs, line, 2, strlen(line)-2);
					lo.num_outputs++;
					break;
				case 'G':
					strcpy(lo.gates[lo.num_gates].name, substr(line, 3, strlen(line)-4));
					switch (line[1]) {
						case 'A':
							lo.gates[lo.num_gates].gate_type = GATE_TYPE_AND;
							break;
						case 'O':
							lo.gates[lo.num_gates].gate_type = GATE_TYPE_OR;
							break;
						case 'X':
							lo.gates[lo.num_gates].gate_type = GATE_TYPE_XOR;
							break;
						default:
							lo.gates[lo.num_gates].gate_type = GATE_TYPE_OR;
							break;

					}
					lo.num_gates++;
				case 'L':
					if (line[1] == 'N' && line[2] == 'K') {
						int q, mid;
						for (q = 4; q < strlen(line); q++) {
							if (line[q] == ' ') {
								mid = q;
								break;
							}
						}

						char beg[500], end[500];
						strcpy(beg, substr(line, 4, (mid-4)));
						strcpy(end, substr(line, mid+1, strlen(line)-(mid+2)));
						printf("BEG AND END /%d///%s/////%s////\n", mid, beg, end);
						struct LocationObject ci;
						struct LocationObject co;
						int done = 0;
						if (strcmp(beg, "POWER") == 0) {
							printf("WE HAVE POWER\n");
							ci = get_connector(end);
							lo.powersource.connections.connectors[lo.powersource.connections.num_outputs].type = ci.type;
							switch (ci.type) {
								case CONNECTOR_TYPE_SWITCH:
									lo.powersource.connections.connectors[lo.powersource.connections.num_outputs].conn.s = &(lo.switches[ci.array_num]);
									done = 1;
									break;
								case CONNECTOR_TYPE_GATE:
									lo.powersource.connections.connectors[lo.powersource.connections.num_outputs].conn.g = &(lo.gates[ci.array_num]);
									done = 1;
									break;
								case CONNECTOR_TYPE_OUTPUT:
									lo.powersource.connections.connectors[lo.powersource.connections.num_outputs].conn.o = &(lo.outputs[ci.array_num]);
									done = 1;
									break;
								default:
									break;
							}
							if (done == 1)
								lo.powersource.connections.num_outputs++;
						} else {
							co = get_connector(beg);
							ci = get_connector(end);
							/* DISCLAIMER:
							 * Yes, I understand this is very shoddy code, and I should have just used pointers.
							 * However, I wrote this code quite late at night, and I just wanted to get this ove
							 * r and done with. In the future, I may return and do this properly, but for now, t
							 * his is it.
							 */
							printf("CONNECTOR TYPE %d (%d) // %d (%d)\n", co.type, co.array_num, ci.type, ci.array_num);
							switch (co.type) {
								case CONNECTOR_TYPE_SWITCH:
									lo.switches[co.array_num].connections.connectors[lo.switches[co.array_num].connections.num_outputs].type = ci.type;
									switch (ci.type) {
										case CONNECTOR_TYPE_SWITCH:
											lo.switches[co.array_num].connections.connectors[lo.switches[co.array_num].connections.num_outputs].conn.s = &(lo.switches[ci.array_num]);
											done = 1;
											break;
										case CONNECTOR_TYPE_GATE:
											lo.switches[co.array_num].connections.connectors[lo.switches[co.array_num].connections.num_outputs].conn.g = &(lo.gates[ci.array_num]);
											done = 1;
											break;
										case CONNECTOR_TYPE_OUTPUT:
											lo.switches[co.array_num].connections.connectors[lo.switches[co.array_num].connections.num_outputs].conn.o = &(lo.outputs[ci.array_num]);
											done = 1;
											break;
										default:
											break;
									}
									if (done == 1)
										lo.switches[co.array_num].connections.num_outputs++;
									break;
								case CONNECTOR_TYPE_GATE:
									lo.gates[co.array_num].connections.connectors[lo.gates[co.array_num].connections.num_outputs].type = ci.type;
									switch (ci.type) {
										case CONNECTOR_TYPE_SWITCH:
											lo.gates[co.array_num].connections.connectors[lo.gates[co.array_num].connections.num_outputs].conn.s = &(lo.switches[ci.array_num]);
											done = 1;
											break;
										case CONNECTOR_TYPE_GATE:
											lo.gates[co.array_num].connections.connectors[lo.gates[co.array_num].connections.num_outputs].conn.g = &(lo.gates[ci.array_num]);
											done = 1;
											break;
										case CONNECTOR_TYPE_OUTPUT:
											lo.gates[co.array_num].connections.connectors[lo.gates[co.array_num].connections.num_outputs].conn.o = &(lo.outputs[ci.array_num]);
											done = 1;
											break;
										default:
											break;
									}
									if (done == 1)
										lo.gates[co.array_num].connections.num_outputs++;
									break;
								case CONNECTOR_TYPE_OUTPUT:
									lo.outputs[co.array_num].connections.connectors[lo.outputs[co.array_num].connections.num_outputs].type = ci.type;
									switch (ci.type) {
										case CONNECTOR_TYPE_SWITCH:
											lo.outputs[co.array_num].connections.connectors[lo.outputs[co.array_num].connections.num_outputs].conn.s = &(lo.switches[ci.array_num]);
											done = 1;
											break;
										case CONNECTOR_TYPE_GATE:
											lo.outputs[co.array_num].connections.connectors[lo.outputs[co.array_num].connections.num_outputs].conn.g = &(lo.gates[ci.array_num]);
											done = 1;
											break;
										case CONNECTOR_TYPE_OUTPUT:
											lo.outputs[co.array_num].connections.connectors[lo.outputs[co.array_num].connections.num_outputs].conn.o = &(lo.outputs[ci.array_num]);
											done = 1;
											break;
										default:
											break;
									}
									if (done == 1)
										lo.outputs[co.array_num].connections.num_outputs++;
									break;
							}
						}
					}
					break;
			}
		}
		fclose(file);
	} else {
		perror(filename);
	}


	/*lo.powersource.connections.connectors[0].type = CONNECTOR_TYPE_SWITCH;
	lo.powersource.connections.connectors[1].type = CONNECTOR_TYPE_SWITCH;
	lo.powersource.connections.num_outputs = 2;

	lo.switches[0].connections.connectors[0].type = CONNECTOR_TYPE_GATE;
	lo.switches[1].connections.connectors[0].type = CONNECTOR_TYPE_GATE;
	lo.switches[0].connections.num_outputs = 1;
	lo.switches[1].connections.num_outputs = 1;

	lo.gates[0].gate_type = GATE_TYPE_XOR;
	lo.gates[0].connections.num_outputs = 1;
	lo.gates[0].connections.connectors[0].type = CONNECTOR_TYPE_OUTPUT;

	lo.powersource.connections.connectors[0].conn.s = &(lo.switches[0]);
	lo.powersource.connections.connectors[1].conn.s = &(lo.switches[1]);
	lo.switches[0].connections.connectors[0].conn.g = &(lo.gates[0]);
	lo.switches[1].connections.connectors[0].conn.g = &(lo.gates[0]);
	lo.gates[0].connections.connectors[0].conn.o = &(lo.outputs[0]);

	strcpy(lo.switches[0].name, "InputUn");
	strcpy(lo.switches[1].name, "InputDe");
	strcpy(lo.outputs[0].name, "Output");

	lo.num_outputs = 1;
	lo.num_switches = 2;
	lo.num_gates = 1;*/

	return 1;
}