PAV - P3: estimación de pitch
=============================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la estimación de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.
``` 
void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {
      r[l]=0;
      for(unsigned int n = l; n<x.size(); n++){
        r[l]+=x[n]*x[n-l];
      }
      r[l]/=x.size();
    }

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
} 
  ```

   * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un segmento de
     unos 30 ms de un fonema sonoro y su periodo de pitch; y, en otro *subplot*, se vea con claridad la
	 autocorrelación de la señal y la posición del primer máximo secundario.
![autocorrelacion](https://user-images.githubusercontent.com/101046951/163352716-404fd1bf-3195-4b9f-89fd-cb34fa29fa2d.jpg)

	 NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
	 hacerlo. Se valorará la utilización de la biblioteca matplotlib de Python.

   * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la
     autocorrelación. Inserte a continuación el código correspondiente.  
 ```
for (iR=iRMax=r.begin()+npitch_min;iR<r.begin()+npitch_max;iR++){
    if(*iR>*iRMax) iRMax=iR;
}
unsigned int lag = iRMax - r.begin();
```

   * Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.
```
  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const {
    if(rmaxnorm>umaxnorm) return false;
    return true;
  }
```

  La regla de decisión que hemos aplicado consiste en comparar el cociente (rmaxnorm) entre el valor de la autocorrelación en 0 (la potencia de la trama) y el valor de la autocorrelación en el pitch con un umbral que hemos introducido. Esta hipotesis se deduce a partir de que para tramas sonoras, la autocorrelación es periodica, y en el lugar donde esta el pitch habrá un máximo, mientras que para tramas sordas, la autocorrelación no es periodica y no hay ningún máximo porque no hay pitch.
     
  El valor más eficiente del umbral lo hemos encontrado comparando distintas realizaciones:
  <img width="735" alt="image" src="https://user-images.githubusercontent.com/101046951/163455676-ac676dcc-9696-4517-8ddb-ea294f56631a.png">

  El umbral más eficiente es 0.38

- Una vez completados los puntos anteriores, dispondrá de una primera versión del estimador de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la estimación de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		(r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).
		
		![image](https://user-images.githubusercontent.com/101046951/163844753-ca558b5c-70e4-4829-8f65-b7629b2d806e.png)


		Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	    Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

      - Use el estimador de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
	    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica
		ilustrativa del resultado de ambos estimadores.
		
		![image](https://user-images.githubusercontent.com/101046951/163847044-3f7be367-9c04-4e79-ad80-3bf1b2b7a775.png)


     
		Aunque puede usar el propio Wavesurfer para obtener la representación, se valorará
	 	el uso de alternativas de mayor calidad (particularmente Python).
  
  * Optimice los parámetros de su sistema de estimación de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..

Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  estimador a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del estimador. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la estimación de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.
    
    <img width="513" alt="image" src="https://user-images.githubusercontent.com/101046951/163445418-cf4b8e20-dc32-4b15-af76-a7c10bf6ae67.png">


- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de estimación
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:

  * Técnicas de preprocesado: filtrado paso bajo, diezmado, *center clipping*, etc.
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el estimador, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al estimador. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.
  
  - Preprocesado: *center clipping*
  
  Con esta técnica recortamos la parte central de la señal, es una forma de limpiar la señal y eliminar la información irrelevante para el pitch, de esta manera se consigue que el porcesado de cada trama sea más eficiente. De las dos variaciones que existen del center clipping hemos decidido usar la variante sin ofset.
  ```
  float umbral_cc = ucc;
  for (unsigned int i = 0; i < x.size(); i++){
    if (abs(x[i]) < umbral_cc){
      x[i] = 0;
    }
    if (x[i] > umbral_cc){
      x[i] = x[i] - umbral_cc;
    }
    if (x[i] < - umbral_cc){
      x[i] = x[i] + umbral_cc;
    }
  }
  ```
  ![center clipping](https://user-images.githubusercontent.com/101046951/163450008-c21576ac-9f2a-4134-a6f8-d7df841288ca.jpg)
  En esta gráfica se puede apreciar el recorte que se le ha aplicado a la señal, sobretodo en los tramos de silencio.

  Para encontar el valor adecuado para el umbral de recorte hemos definido el parametro ucc usando la librería `docopt_cpp` para comparar distintas realizaciones, este ha sido nuestro resultado:
  <img width="737" alt="image" src="https://user-images.githubusercontent.com/101046951/163450623-3bff76cc-62a5-41c8-bb65-a44226267ac0.png">
  
  El umbral más eficiente para el center-clipping es 0.0065.
  
  
  - Postprocesado: filtro de mediana
  
  Con el objetivo de corregir errores de estimación del pitch, en concreto aquellos valores del pitch que sobrepasan un margen de coherencia en comparación con sus tramas vecinas, se utiliza el filtro de mediana. Este filtro compara el pitch de una trama con sus vecinos y se queda con el valor de la mediana. Puede ser de orden 3, 5 o 7. El más eficiente es el de orden 3, que es el que hemos utilizado.
  ```
  vector<float> f0m(f0.size());
  f0m = f0;
  for (unsigned int j = 1; j < f0.size() - 1; j++){
    vector<float> vect = {f0[j-1], f0[j], f0[j+1]};
    sort (vect.begin(), vect.end());
    f0m[j] = vect[1];
  }
  f0 = f0m;
  ```
  Con el filtro obtenemos un resultado de:
  
  <img width="466" alt="image" src="https://user-images.githubusercontent.com/101046951/163453551-2acbee64-0f7a-41c3-93b7-92beed183f68.png">

  Sin el filtro obtenemos:
  
  <img width="467" alt="image" src="https://user-images.githubusercontent.com/101046951/163453712-feabed6c-4874-4378-be05-93cfee3fafb8.png">
  
  Si ampliamos la dimensión del filtor a 5, obtenemos el siguiente score:
  
  <img width="466" alt="image" src="https://user-images.githubusercontent.com/101046951/163460832-51d8f21c-ea68-4dbb-ac9a-161a5cf709ca.png">

  Es mejor no utilizar filtro a utilizar un filtro de orden 5, no obstante la mejor opción es el filtro de orden 3.

  
  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   

Evaluación *ciega* del estimador
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
estimador con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
