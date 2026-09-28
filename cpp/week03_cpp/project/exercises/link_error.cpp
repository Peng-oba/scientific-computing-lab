double response(double strain);  // Declared, but never defined.

int main()
{
    return response(0.01) == 10.0 ? 0 : 1;
}
