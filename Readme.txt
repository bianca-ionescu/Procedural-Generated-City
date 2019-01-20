Ionescu Bianca-Iulia
334CB
Tema 3 EGC


In implementarea temei am pornit de la laboratorul 9, dupa cum detaliez mai 
jos:

Fragment Shader:
- ca si in lab9, pentru prelucrarea texturilor

Vertex Shader:
- ca si in lab9, dar combinat cu lab7 in incercarea de a face o iluminare 
(nestiind cum sa calculez culoarea, nu prea a iesit)

.h:
- declarare constante iluminare + constante ale matricii de desenare
- declararea unei constante SECTION reprezentand dimensiunea matricii n x n

.cpp:
	Init:
	- declarare texturi
	- incarcare mesh-uri
	- creare quad
	- incarcare shadere
	- initializarea matricii de drum: pentru a respecta cerinta, am folosit o 
	serie de variabile random pentru stabilirea matricii de drum: aceasta este 
	fie orizontala, fie verticala (o serie de linii paralele), ce in unele 
	cazuri ajunge sa fie o retea efectiva de drumuri
	- alegerea modelelor cladirilor, pentru varietate
	- initializarea matricii de scalare, folosita in update
	- declarare proprietati de lumina si material

	Update:
	- desenare si texturare drum, "camp", cladiri (ambele nivele - aici variem 
	dimensiunile prin matricea de scalare si a unei constante de scalare)
	- desenarea si texturarea unei animatii - cea a norilor, care datorita 
	functiei random apar si dispar. Ca in viata
	- desenarea punctului de iluminare - soarele, si texturarea lui

	RenderSimpleMesh:
	- am incercat a-l edita (combinarea laburilor 7 si 9) pentru a realiza 
	iluminarea; am ramas la o singura textura, deoarece stim deja de la lab ca 
	2 esueaza
	- am comentat variabila de culoare deoarece este nefunctionala si da eroara 
	in stadiul actual

	OnInputUpdate:
	- deplasarea prin "harta"

	OnKeyPress:
	- schimbarea "fortata" a nivelului 2 al cladirilor - mai exact a matricii 
	de scalare