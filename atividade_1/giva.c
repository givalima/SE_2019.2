#include "giva.h"
// Atividade 01 - Sistemas Embarcados 2019.2

enum rooms_house { ROOM = 0, KITCHEN = 1, BEDROOM = 2, BWC = 3};
enum voice_command {VOICE_COMMAND1 = 20, VOICE_COMMAND2 = 11};
enum tasks {LIGHTING = 0, SOUND = 1, AIR = 2, WINDOWS = 3, DOOR = 4, TV = 5};
enum commands {VOICE = '0', APP = '1'};
enum stats {OFF = 0, ON = 1};


int smart_house_device_init(device *home_d, char *user, char wifi, unsigned int content)
{
     if (home_d == NULL || user == NULL){
        return 0; 
    } 
    strcpy(home_d->user, user);
    home_d->connection_wifi = wifi;
    home_d->mode = '1';
    home_d->current_command.content = content;
    return 1;
}

void house_init(my_house *house)
{
    if (house == NULL)
    {
        return;
    }
    memset(house->room, 0, sizeof(house->room));
    memset(house->bwc, 0, sizeof(house->bwc));
    memset(house->bedroom, 0, sizeof(house->bedroom));
    memset(house->kitchen, 0, sizeof(house->kitchen));
}

int smart_house_device_isconnected(device *home_d){
    if(home_d->connection_wifi == ON){
       return 1;
    }
   return 0;
}

void smart_house_device_print(device *home_d)
{
    if(home_d == NULL)
    {
        return;
    }
    printf("Username: %s\n", home_d->user);
    switch (home_d->connection_wifi){
    case 0:
        printf("device System is not Connected.\n");
        break;
    case 1:
        printf("device System is Connected!\n");
        break;
    default:
        printf("[ERROR] something is wrong =(.\n");
        break;
    }
}

int check_voice_command(command *cmd){
    if(cmd->voice_command.val == 5 || cmd->voice_command.val == 6){
        return 1;
    }   
    return 0;
}

void smart_house_device_execute(device *home_d, my_house *house, char mode, command *cmd){
    if(smart_house_device_isconnected(home_d) == 1){
        if(mode == VOICE){
            if(check_voice_command(cmd)){
                if(cmd->voice_command.interpret == VOICE_COMMAND1){ //"ligar luzes da sala e abrir janela da cozinha"
                    house -> room[LIGHTING] = ON;
                    house -> kitchen[WINDOWS] = ON;
                }
                else if(cmd->voice_command.interpret == VOICE_COMMAND2){ //"ligar luz do banheiro e fechar a porta"
                    house -> bwc[LIGHTING] = ON;
                    house -> bwc[DOOR] = OFF;
                }
            }
        }
        else if(mode == APP){
            char* room_house;
            switch(cmd->app_command.place_code){
                case ROOM:
                    room_house = house->room;
                    break;

                case KITCHEN:
                    room_house = house->kitchen;
                    break;

                case BEDROOM:
                    room_house = house->bedroom;
                    break;
                
                case BWC:
                    room_house = house->bwc;
                    break;
                
                default:
                    break;
            }

            room_house[LIGHTING] = cmd->app_command.lighting;
            room_house[SOUND] = cmd->app_command.sound;
            room_house[AIR] = cmd->app_command.air_conditioning;
            room_house[WINDOWS] = cmd->app_command.windows;
            room_house[DOOR] = cmd->app_command.door;
            room_house[TV] = cmd->app_command.tv;
        }
    }
    else{
        printf("Check if your wi-fi is connected!");
    }
}

/*void smart_house_device_print_binary_content(unsigned int cmd){

}*/

void smart_house_device_monitor(my_house *house){
    /* Elabore aqui um printf que siga a representacao abaixo: */
    // ## MONITOR 
    // # (AMBIENTE): 
    // >> ILUMINAÇÃO: (1 ou 0)
    // >> SOM AMBIENTE: (1 ou 0)
    // >> AR CONDICIONADO: (1 ou 0)
    // >> JANELAS: (1 ou 0)
    // >> PORTA: (1 ou 0)
    // >> TV: (1 ou 0)
    // ---------- * ----------

    printf("## MONITOR\n# (SALA):\n");
    smart_house_device_monitor_print(house->room);
    printf("# (COZINHA):\n");
    smart_house_device_monitor_print(house->kitchen);
    printf("# (QUARTO):\n");
    smart_house_device_monitor_print(house -> bedroom);
    printf("# (BANHEIRO):\n");
    smart_house_device_monitor_print(house -> bwc);
    printf("\n\n");
}

void smart_house_device_monitor_print(char *room_name){
    printf(">> ILUMINAÇÃO: %d\n", room_name[LIGHTING]);
    printf(">> SOM AMBIENTE: %d\n", room_name[SOUND]);
    printf(">> AR CONDICIONADO: %d\n", room_name[AIR]);
    printf(">> JANELAS: %d\n", room_name[WINDOWS]);
    printf(">> PORTA: %d\n", room_name[DOOR]);
    printf(">> TV: %d\n", room_name[TV]);
    printf("---------- * ----------\n");
}

int main(int argc, char const *argv[])
{
    // Declare
    device home_d;
    my_house house;
    command cmd = {0};

    // Inicialize
    //smart_house_device_init(&home_d, "<seu_nome>", '1', cmd.content);
    //house_init(&house);
    smart_house_device_init(&home_d, "GIVANILDO", ON, cmd.content);
    house_init(&house);

    // Verifique
    smart_house_device_print(&home_d);
    smart_house_device_monitor(&house);
    
    // Obs.: Lembre-se sempre verificar se o dispositivo esta conectado ao wi-fi antes de executar alguma acao
    // Obs.: Seu codigo voz eh 101, sempre verifique antes de executar um comando de voz

    /* EXECUTE: OK
    // Comando de voz
    /// (codigo de voz) : 101
    /// (comando) : 10100 == "ligar luzes da sala e abrir janela da cozinha"
    // smart_house_device_execute();
    // smart_house_device_monitor();
    */

    cmd.voice_command.val = 5; 
    cmd.voice_command.interpret = 20;
    smart_house_device_execute(&home_d, &house, VOICE, &cmd);
    smart_house_device_monitor(&house);

    /* EXECUTE:
        // Comando de app
        // (ambiente) sala
        /// ligar luzes + som ambiente + abrir janelas + fechar porta
    // smart_house_device_execute();
    // smart_house_device_monitor();
    */

    memset(&cmd.app_command, 0, sizeof(cmd.app_command));
    cmd.app_command.place_code = ROOM;
    cmd.app_command.lighting = ON;
    cmd.app_command.sound = ON;
    cmd.app_command.windows = ON;
    cmd.app_command.door = OFF;
    smart_house_device_execute(&home_d, &house, APP, &cmd);
    smart_house_device_monitor(&house);

    /* EXECUTE: OK
    // Comando de voz
    /// (codigo de voz) : 110
    /// (comando) : 01011 == "ligar luz do banheiro e fechar a porta"
    // smart_house_device_execute();
    // smart_house_device_monitor();
    */

    cmd.voice_command.val = 6; 
    cmd.voice_command.interpret = 11;
    smart_house_device_execute(&home_d, &house, VOICE, &cmd);
    smart_house_device_monitor(&house);

    /* EXECUTE:
        // Comando de app
        // (ambiente): quarto
        /// ligar luz +  ar condicionado + fechar janela + abrir porta
    // smart_house_device_execute();
    // smart_house_device_monitor();
    */

    memset(&cmd.app_command, 0, sizeof(cmd.app_command));
    cmd.app_command.place_code = BEDROOM;
    cmd.app_command.lighting = ON;
    cmd.app_command.air_conditioning = ON;
    cmd.app_command.windows = OFF;
    cmd.app_command.door = ON;
    smart_house_device_execute(&home_d, &house, APP, &cmd);
    smart_house_device_monitor(&house);


    /* EXECUTE: OK
    // Comando de voz
    /// (codigo de voz) : 101
    /// (comando) : 01011 == "ligar luz do banheiro e fechar a porta"
    // smart_house_device_execute();
    // smart_house_device_monitor();
    */

    cmd.voice_command.val = 5; 
    cmd.voice_command.interpret = 11;
    smart_house_device_execute(&home_d, &house, VOICE, &cmd);
    smart_house_device_monitor(&house);

    return 0;
}
