#include <main.h>
#include <error.h>

/* TODO
 * 	Add specific error messages
 * 	Check args are valid (numbers > 0)
*/

static int check_input(int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
	{
		return (0);
	}
	(void**)argv;
	return (1);
}

int main(int argc, char *argv[])
{
	if (!check_input(argc, argv))
	{
		return (ERROR_INPUT);
	}

	// Initialize

	// Start threads

	// Clear up

	return (0); // Consider returning status
}