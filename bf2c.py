buffer_size = 10000

def isValid(code):
    stack = 0
    for c in code:
        if c == '[':
            stack += 1
        elif c == ']':
            stack -= 1
        if stack < 0:
            return False
    return True

def generate_intermediate(code):
    stack = []
    for c in code:
        if c == '+' and (len(stack) == 0 or stack[-1][0] != '+'):
            stack.append(['+', 1])
        elif c == '-' and (len(stack) == 0 or stack[-1][0] != '+'):
            stack.append(['+', -1])
        elif c == '>' and (len(stack) == 0 or stack[-1][0] != '>'):
            stack.append(['>', 1])
        elif c == '<' and (len(stack) == 0 or stack[-1][0] != '>'):
            stack.append(['>', -1])
        elif len(stack) == 0:
            stack.append([c, None])
        elif stack[-1][0] == '+' and c == '+':
            stack[-1][1] += 1
            if stack[-1][1] == 0:
                stack.pop()
        elif stack[-1][0] == '+' and c == '-':
            stack[-1][1] -= 1
            if stack[-1][1] == 0:
                stack.pop()
        elif stack[-1][0] == '>' and c == '>':
            stack[-1][1] += 1
            if stack[-1][1] == 0:
                stack.pop()
        elif stack[-1][0] == '>' and c == '<':
            stack[-1][1] -= 1
            if stack[-1][1] == 0:
                stack.pop()
        elif c in '[].,':
            stack.append([c, None])
    return stack

def generate(code):
    if not isValid(code):
        print('syntax error')
        return
    code = generate_intermediate(code)
    print(code)
    with open('a.c', 'w') as f:
        f.write(f'#include <stdio.h>\nvoid main(){{\n\tchar buff[{buffer_size}] = {{}};\n\tint ptr = {buffer_size // 2};\n')
        tab_count = 1
        for c, count in code:
            if c == '+':
                f.write('\t' * tab_count)
                f.write(f'buff[ptr] += {count};\n')
            elif c == '>':
                f.write('\t' * tab_count)
                f.write(f'ptr += {count};\n')
            elif c == '[':
                f.write('\t' * tab_count)
                f.write('do{\n')
                tab_count += 1
            elif c == ']':
                tab_count -= 1
                f.write('\t' * tab_count)
                f.write('}while(buff[ptr] != 0);\n')
            elif c == '.':
                f.write('\t' * tab_count)
                f.write('printf("%c", buff[ptr]);\n')
            elif c == ',':
                f.write('\t' * tab_count)
                f.write('scanf("%c", &buff[ptr]);\n')
        f.write('}')

generate(input())