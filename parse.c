#include <stdio.h>
#include <stdlib.h>

// Right. We have a struct called Input. In Input we have one power source, and connectors with that. Then connectors to those. And so on.



int get_config(void) {
	lo.powersource.connections.connectors[0].type = CONNECTOR_TYPE_SWITCH;
	lo.powersource.connections.connectors[0].conn.s = &(lo.switches[0]);
	lo.powersource.connections.num_outputs = 1;
	strcpy(lo.switches[0].name, "Switch");
	lo.switches[0].connections.connectors[0].type = CONNECTOR_TYPE_OUTPUT;
	lo.switches[0].connections.connectors[0].conn.o = &(lo.outputs[0]);
	lo.switches[0].connections.num_outputs = 1;
	strcpy(lo.outputs[0].name, "Out");
	lo.num_switches = 1;
	lo.num_outputs = 1;
	return 1;
}