#include <MDE/CommandLine.h>

int MDECommandLineParse(MDECommandLine* param, int count, int argc, char** argv){
	int i;
	for(i = 1; i < argc; i++){
		int j;
		for(j = 0; j < count; j++){
			char* s = malloc(1 + strlen(param[j].short_opt) + 1);
			char* l = malloc(2 + strlen(param[j].long_opt) + 1);

			strcpy(s, "-");
			strcat(s, param[j].short_opt);

			strcpy(l, "--");
			strcat(s, param[j].long_opt);

			if(strcmp(s, argv[i]) == 0 || strcmp(l, argv[i]) == 0){
				free(l);
				free(s);

				if(param[j].need_arg && (i + 1) >= argc){
					fprintf(stderr, "%s: %s needs 1 argument\n", argv[0], argv[i]);
					return 1;
				}else if(param[j].need_arg){
					param[j].call(argv[++i]);
				}else{
					param[j].call(NULL);
				}

				break;
			}else{
				free(l);
				free(s);
			}
		}
	}

	return 0;
}
