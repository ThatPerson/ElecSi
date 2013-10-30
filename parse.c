#include <stdio.h>
#include <stdlib.h>

// Right. We have a struct called Input. In Input we have one power source, and connectors with that. Then connectors to those. And so on.



int get_config(void) {
	/*lo.powersource.connections.connectors[0].type = CONNECTOR_TYPE_SWITCH;
	lo.powersource.connections.connectors[0].conn.s = &(lo.switches[0]);
	lo.powersource.connections.num_outputs = 1;
	strcpy(lo.switches[0].name, "Switch");
	lo.switches[0].connections.connectors[0].type = CONNECTOR_TYPE_OUTPUT;
	lo.switches[0].connections.connectors[0].conn.o = &(lo.outputs[0]);
	lo.switches[0].connections.num_outputs = 1;
	strcpy(lo.outputs[0].name, "Out");
	lo.num_switches = 1;
	lo.num_outputs = 1;*/



	lo.powersource.connections.connectors[0].type = CONNECTOR_TYPE_SWITCH;
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
	lo.num_gates = 1;

	return 1;
}