#include <stdio.h>
#include <stdlib.h>

// Right. We have a struct called Input. In Input we have one power source, and connectors with that. Then connectors to those. And so on.

struct Input {
	struct Power powersource;
	struct Switch switches[500];
	struct Output outputs[500];
	struct Gate gates[500];
	int num_switches;
	int num_outputs;
	int num_gates;
};

Input get_config(void) {
	struct Input test_case;
	test_case.powersource.connectors.connection.type = CONNECTOR_TYPE_SWITCH;
	test_case.powersource.connectors.connection.conn.s = &(test_case.switches[0]);
	strcpy(test_case.switches[0].name, "Switch");
	test_case.switches[0].connectors.connection.type = CONNECTOR_TYPE_OUTPUT;
	test_case.switches[0].connectors.connection.conn.o = &(test_case.outputs[0]);
	return test_case;
}