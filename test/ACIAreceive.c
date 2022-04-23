#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int main() {
	char message[1000];

  n_printf("ACIA_receive : En attente de la reception d'un message.....\n");
	int res = TtyReceive(message,1000);
	n_printf("message reçu : \"%s\"\nRésultat de TtyReceive : %d", message, res);

	return 0;
}
