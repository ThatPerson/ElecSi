char * substr(char * orig, int start, int length) {
        char * ret = (char *)malloc (sizeof (char) * length);
        strncpy (ret, orig+start, length);
        ret[length] = '\0';
        return ret;
}

int intlength(int l) {
        if (l != 0) {
                return floor(log10(abs(l))) + 1;
        }
        return -1;
}

struct Connection * find_connection(char * name, struct Test * r) {
  int i;
  struct Connection * ret;
  for (i = 0; i < r->num_output; i++) {
    if (strcmp(r->o[i].name, name) == 0) {
      ret = &r->o[i].connections;
     
    }
  }
  for (i = 0; i < r->num_power; i++){
    if (strcmp(r->p[i].name, name) == 0) {
      ret = &r->p[i].connections;
    }
  }
  for (i = 0; i < r->num_switch; i++){
    if (strcmp(r->s[i].name, name) == 0) {
      ret = &r->s[i].connections;
    }
  }
  for (i = 0; i < r->num_gate; i++){
    if (strcmp(r->g[i].name, name) == 0) {
      ret = &r->g[i].connections;
    }
  }
  return ret;
}

struct Connector find_connector(char name[], struct Test * r) {
  int i;
  struct Connector ret;
  ret.type = -1;
  for (i = 0; i < r->num_output; i++) {
    if (strcmp(r->o[i].name, name) == 0) {
      ret.conn.o = &r->o[i];
      ret.type = CONNECTOR_TYPE_OUTPUT;
    }
  }
  for (i = 0; i < r->num_power; i++){
    if (strcmp(r->p[i].name, name) == 0) {
      ret.conn.p = &r->p[i];
      ret.type = CONNECTOR_TYPE_POWER;
    }
  }
  for (i = 0; i < r->num_switch; i++){
    if (strcmp(r->s[i].name, name) == 0) {
      ret.conn.s = &r->s[i];
      ret.type = CONNECTOR_TYPE_SWITCH;
    }
  }
  for (i = 0; i < r->num_gate; i++){
    if (strcmp(r->g[i].name, name) == 0) {
      ret.conn.g = &r->g[i];
      ret.type = CONNECTOR_TYPE_GATE;
    }
  }
  printf("%d\n", ret.type);
  return ret;
}

struct Test get_config(char filename[]) {
        struct Test r;
        FILE * file = fopen(filename, "r");
        r.num_gate = 0;
        r.num_output = 0;
        r.num_power = 0;
        r.num_switch = 0;
        if (file != NULL) {
                char line[128];
                while (fgets(line, sizeof(line), file) != NULL) {
                        switch (line[0]) {
                          case 'G':
                            switch (line[1]) {
                              case 'A':
                                r.g[r.num_gate].gate_type = GATE_TYPE_AND; break;
                              case 'X':
                                r.g[r.num_gate].gate_type = GATE_TYPE_XOR; break;
                              case 'O':
                                r.g[r.num_gate].gate_type = GATE_TYPE_OR; break;
                              default:
                                printf("There may be a error in your sim file.");
                                exit(0);
                            }
                            strcpy(r.g[r.num_gate].name, substr(line, 3, strlen(line)-4));
                            r.g[r.num_switch].connections.num_outputs = 0;

                            r.num_gate++;
                            break;
                          case 'S':
                            r.s[r.num_switch].value = 0;
                            r.s[r.num_switch].connections.num_outputs = 0;
                            strcpy(r.s[r.num_switch].name, substr(line, 2, strlen(line)-3));
                            r.num_switch++;
                            break;
                          case 'O':
                            strcpy(r.o[r.num_output].name, substr(line, 2, strlen(line)-3));
                            r.o[r.num_output].value = 0;
                            r.o[r.num_switch].connections.num_outputs = 0;
                            r.num_output++;
                            
                            break;
                          case 'P':
                            strcpy(r.p[r.num_power].name, substr(line, 2, strlen(line)-3));
                            r.p[r.num_switch].connections.num_outputs = 0;

                            r.num_power++;
                            break;
                          case 'L':
                            if (line[1] == 'N') {
                              if (line[2] == 'K') {
                                int i, found = 0;
                                struct Connection * out;
                                struct Connector in;
                                for (i = 4; i < strlen(line); i++) {
                                  if (line[i] == ' ') {
                                    found = 1;
                                    char * goingout;
                                    char * goingin;
                                    goingout = (char *) malloc(i-4 * sizeof(char));
                                    goingin = (char *) malloc((strlen(line) - i) * sizeof(char));
                                    strcpy(goingout, substr(line, 4, i-4));
                                    strcpy(goingin, substr(line, i+1, strlen(line)-(i)-2));
                                    out = find_connection(goingout, &r);
                                    //printf("OUT: %d %s\n", out->num_outputs, goingout);
                                    in = find_connector(goingin, &r);
                                    //printf("IN: %d %s\n", in->type, goingin);
                                    
                                    int ll;
                                    for (ll = 0; ll < strlen(goingout); ll++) {
                                      printf("%d: %d (%c)\n", ll, goingout[ll], goingout[ll]);
                                    }
                                    printf("\n");
                                    for (ll = 0; ll < strlen(goingin); ll++) {
                                      printf("%d: %d (%c)\n", ll, goingin[ll], goingin[ll]);
                                    }
                                    
                                    free(goingin);
                                    free(goingout);
                                    break;
                                  }
                                }
                                if (found == 1) {
                                  out->connectors[out->num_outputs] = in;
                                  out->num_outputs++;
                                  printf("%d\n", &(out->num_outputs));
                                }
                              }
                            }
                            break;
                        }
                  //fputs(line, stdout);
                }
                fclose(file);
        } else {
                perror(filename);
        }
        return r;
}
