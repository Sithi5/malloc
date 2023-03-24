int main() {
    void *p1 = malloc(16);
    void *p2 = malloc(32);
    void *p3 = malloc(64);
    printf("p1=%p\n", p1);
    printf("p2=%p\n", p2);
    printf("p3=%p\n", p3);
    free(p1);
    free(p3);
    void *p4 = realloc(p2, 128);
    printf("p4=%p\n", p4);
    free(p4);
    return 0;
}