import sys
import BigInt

print "Input arguments in the specified order:"
Arguments = raw_input().split()
s = len(Arguments)
if s < 4:
    print "Insufficient input data!"
    sys.exit(-1)
else:
    file1 = Arguments[0]
    oper = Arguments[1]
    file2 = Arguments[2]
    result = Arguments[3]
    Basis = 10
    mod = ""
    if s == 5:
        if Arguments[4] == "-b":
            Basis = 256
        else:
            mod = Arguments[4]
    elif s == 6:
        Basis = 256
        mod = Arguments[5]

a = BigInt.BigInt()
b = BigInt.BigInt()
a.setbase(Basis)
b.setbase(Basis)
a.scan(file1)
b.scan(file2)

if oper == "+": a = a + b
elif oper == "-": a = a - b
elif oper == ".": a = a * b
elif oper == "/": a = a / b
elif oper == "^": a = a ^ b
elif oper == "%": a = a % b

if mod != "":
	m = BigInt.BigInt()
	m.setbase(Basis)
	m = m.Read(mod)
	a = a % m
    
a._print(result)
