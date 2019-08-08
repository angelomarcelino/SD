def trapmf(x, a, b, c, d, minimo, maximo):
    y = 0

    if x <= a:
        y = minimo
    elif (a <= x) & (x < b):
        y = maximo * (x - a)/(b - a)
    elif (b <= x) & (x < c):
        y = maximo
    elif (c <= x) & (x < d):
        y = maximo * (d - x)/(d - c)
    elif d <= x:
        y = minimo
    
    return int(y)

while True:
    x = input('entre com o ponto desejado: ')
    y = trapmf(int(x), 0, 0, 5, 10, 0, 255)
    print(y)
