# all data assumed little endian

def mask():
    return 0x0F

def first_nibble(value):
    rotate = value >> 4
    return rotate & mask()

def second_nibble(value):
    return value & mask()
    
def hexa(value):
    string = '%02x' % value
    return string.upper()

def one():
    return 1

def zero():
    return 2

def reverse(byte):
    segment = 0
    value = 0
    for i in range (0,4):
        value = value << 2
        segment = byte >> i * 2
        segment = segment & 3
        value = value | segment
    return value

def bigend(byte):
    value = 0
    segment = 0
    for i in range (0,8):
        value <<= 1
        segment = byte >> i 
        segment = segment & 1
        value = value | segment
    return value

def encode(nibble):
    value = 0
    for i in range(0,4):
        value = value << 2
        bit = nibble >> i 
        bit = bit & 1
        if bit == 1:
            value = value | one()
        else:
            value = value | zero()
    return reverse(value)

def sequence(val):
    first = encode(first_nibble(val))
    second = encode(second_nibble(val))
    value = { 'a': first, 'b': second }
    return value



def binary(byte):
    byte = byte & 0xFF
    bigendian = bigend(byte)
    string = ''
    for i in range(0,8):
        bit = bigendian >> i
        bit = bit & 1
        if bit == 1:
            string += '1'
        else:
            string += '0'
    return string

def table(maximum):
    arr = {}
    for i in range(0, maximum):
        arr[i] = sequence(i)
    return arr

def cout(maxrange):
    arr = table(maxrange)
    print "void table(unsigned char index) {"
    print "\tunsigned char buf[2];\n"
    print "\tswitch(index) {"
    for i in range(0,maxrange):
        print "\t\tcase", str(i) + ':'
        print "\t\t\t//" , binary(i), '=>' , binary(arr[i]['a']), ':' , binary(arr[i]['b'])
        print "\t\t\t" + "buf[0] = 0x" + hexa(arr[i]['a']) + ";"
        print "\t\t\t" + "buf[1] = 0x" + hexa(arr[i]['b']) + ";"
        print "\t\t" + "break;"
    print "\t}"
    print "}"

    #reverse-table
    print ""
    print "unsigned char Rtable(unsigned char a, unsigned char b) {"
    print ""
    for i in range(0,maxrange):
        print "\t//", binary(arr[i]['a']), ':' , binary(arr[i]['b']), '=>', binary(i) 
        print "\tif (a == 0x" + hexa(arr[i]['a']), '&& b == 0x' + hexa(arr[i]['b']) + ")"
        print "\t\treturn", str(i) + ";"
        print ""
    print "}"


def main():
    cout(256)

if __name__ == "__main__":
    main()

