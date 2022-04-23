#include "userlib/syscall.h"
#include "userlib/libnachos.h"

char * message = "ARTOUR, j'apprécie les fruits au sirop ! Pas changer assiette pour fromage !";

int main() {

	n_printf("ACIA_send : Envoi d'un message.....\n");
	int res = TtySend(message);
	n_printf("Fin de l'envoi ! résultat de TtySend : %d\n", res);


	return 0;
}
