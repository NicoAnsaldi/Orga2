Rtp2

En el branch Master, estan los filtros y pasan los test comunes y de memoria
Correr haciendo make en la carpeta src, y para un filtro : "./tp2 c rgb2yuv lena.bmp sarasa.bmp"
Se puede elegir c o asm, el nombre de la imagen resultado a eleccion.
Dentro de la carpeta test correr ./1_ , ./2_ etc. para ver que pasan los test.
Esta explicacion la dejo para futuro(uno despues se olvida como correr todos los tps :/)

En el branch Experimentacion:
Esta modificado el tp de la catedra para que el archivo en la carpeta scripts de un archivo .csv 
(se puede modificar a mano para que saque txt en el archivo run.c) por cada filtro
Con 0 si es c, 1 si es asm en la primera columna, la 2da es el tamaño de la imagen, y la 3 la cantidad de ciclos que tardó dicho filtro.

Los 2 experimentos realizados:
Four combine original vs Four Combine con mas accesos de memoria.
Rgb2yuv original vs Rgb2yuv con floats


Dejo aca el link a la tabla de excel donde escribi las mediciones y el share latex, igual subi el informe en formato pdf

https://es.sharelatex.com/project/591df5329fb58ede3dc4a02e

https://docs.google.com/spreadsheets/d/1xqSge--oOyOtWaz9oDsDMJs8znfvpA6X8H9l28Q89yo/edit#gid=410443293
De este ultimo es la hoja 2.

Dejo aca esto para nosotros, o mismo si algun profe pasa a leer.




