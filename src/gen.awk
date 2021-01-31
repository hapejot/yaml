BEGIN {
for(i = 0; i < 1000; i++){
        "uuidgen" | getline guid
        printf("- id: %s\n", guid)
        printf("  name: Name%d\n", i)
        close("uuidgen")
    }
}
