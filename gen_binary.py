print("#ifndef VM_BINARY_H")
print("#define VM_BINARY_H")

for i in range(0, 256):
    print(f"#define b_{bin(i)[2:].zfill(8)} {i}")

print("#endif")
