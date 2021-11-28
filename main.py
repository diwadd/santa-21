
replace_dict = {
 '4': '🎅',
 '3': '🤶',
 '7': '🌟',
 '0': '🦌',
 '1': '🧝',
 '2': '🎄',
 '5': '🎁',
 '6': '🎀'}

f = open("solution_raw.txt")
g = open("mySub.txt", "w")
g.write("schedule\n")
lines = f.readlines()

n = len(lines)
for i in range(n):
    
    lines[i] = lines[i].replace("\n","")

    m = len(lines[i])
    v = []
    for j in range(m):
        v.append(replace_dict[str(int(lines[i][j]))])

    s = "".join(v) + "\n"
    # print(s)
    g.write(s)
g.close()