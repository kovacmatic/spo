#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }


/* Program, ki po pageih izpiše vsebino filea */
int main(int argc, char const *argv[])
{
    // validate argv count
    if (argc != 2) {
        error("Invalid args count! Usage ./more <path_name>")
    }

    

    return 0;
}
