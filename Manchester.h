/*
*  Manchester.h
*   MANCHESTER Interface
*   
*/

#ifndef MANCHESTER_H
#define MANCHESTER_H
#define MANCHESTER_VERSION "0"

class Manchester {
    public:
        unsigned char* encode(unsigned char byte);
        unsigned char decode(unsigned char* encoded);
        void print(const char* string);
        void print(float arg);
        void print(int arg);
    protected:
        unsigned char buffer[2];
        void table(unsigned char index);
        unsigned char Rtable(unsigned char a, unsigned char b);
    private:
};

#endif
