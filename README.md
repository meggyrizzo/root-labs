# root-labs

per compilare g++ main.cpp mcgenerator.cpp functions.cpp `root-config --cflags --libs` -o runMC
per eseguire ./runMC

SCHEDA 1
nel metodo drawfunction l'obbiettivo è generare dopo aver generato un istogramma con MC sovrapporlo alla funzione teorica-> devono avere la stessa scala (stessa area sotto la curvatura)
1. è normalizzato l'istogramma a 1
2. creo funzione teorica f_norm
3. calcolo area sotto la funzione teorica (double integral_f = f_norm.Integral(x_min, x_max);) -> così conosco il peso della funzione nel range scelto
4. double scale_factor = integral_h / integral_f; questo è il coefficiente moltiplicativo x rendere la funzione teorica confrontabile con l'istogramma (se la funzione ha area 2 e l’istogramma ha area 1, il fattore sarà 0.5 → abbassa la funzione)
5. creo nuova funzione e la scalo per il fattore di scala appena ottenuto
6. disegno h e f_scaled nello stesso canvas


da fare
- confronto tra parametri fissi e variabili con emtodo 3.2 e metodo 3.3
- aggiungere alla "legenda" lo scarto quadratico medio
- creare file con funzioni usate piu volte


SCHEDA 2
1. mostrare nel plot le incertezze-> si vede gia dalle barre di errore
2. stampare le incertezze su file o terminale, da fare
io l'ho inteso come uno stampare media e dev standard per ogni bin per ogni grafico-> ho creato 4 file di output uno per ogni TGraphError dove sono indicati per ogni bin il valore della media e della deviazione standard 
-------------------------------------------------------------------
ora considero davvero solo il grafico generato nel punto 3.2 (con incertezza da rigenerazione e con parametri k, b, teta senza incertezza)
-> fit di quello a parametri fissi + calcolare resido (cambiera sempre al variare di N_replicas)

variando parametri fit (parametri con incertezza credo) vedi come varia residuo, questo punto l'ho inteso come un non dare per fissati i parametri b, teta e k e vedere come varia il residuo, qui mi aspetto che b, teta, k cambino sempre dato che la replica non sara mai perfettamente uguale

Punto 3: Fit della funzione a parametri fissati
Cosa si intende:

Eseguire un fit della distribuzione generata (l'istogramma degli eventi MC) usando la funzione 
𝑓
(
𝑥
)
=
cos
⁡
2
(
𝑘
𝑥
+
𝜙
)
+
𝑏
 con i parametri fissati ai valori nominali: 
𝑘
=
5.2
, 
𝜙
=
1.8
, 
𝑏
=
0.2
.

In ROOT, questo si traduce in creare una TF1 con parametri non variabili e confrontarla con l'istogramma.

Obiettivo:

Verificare visivamente e numericamente quanto bene la funzione teorica descrive i dati simulati.

Non si cerca ancora di ottimizzare i parametri, ma solo di confrontare.


Punto 4: Calcolo dei residui (distanza tra fit e distribuzione)
Cosa si intende:

Implementare una funzione che calcoli la differenza tra il valore del fit e il valore dell’istogramma in ogni bin.

Tipicamente:

𝑟𝑖=ℎ𝑖−𝑓(𝑥𝑖) dove ℎ𝑖 è il contenuto del bin 𝑖, e 𝑓(𝑥𝑖) è il valore della funzione nel centro del bin.

Possibili varianti:

Residui assoluti: 
𝑟𝑖

Residui normalizzati: 
𝑟𝑖/𝜎𝑖, dove 𝜎𝑖 è l’incertezza del bin

Somma dei quadrati dei residui: utile per valutare la bontà del fit

Obiettivo:

Quantificare l’accordo tra funzione e dati.

Preparare il terreno per il punto successivo.


Punto 5: Variazione dei parametri e studio dell’accordo
Cosa si intende:

Variare i parametri 
𝑘
,
𝜙
,
𝑏
 manualmente o tramite fit e osservare come cambia l’accordo con i dati.

Puoi:

Fare un fit con parametri liberi e confrontare con quello a parametri fissi.

Variare i parametri in un range (es. 𝑘=5.0 → 5.4) e calcolare i residui per ciascun caso.

Visualizzare l’andamento della somma dei residui o del chi-quadro.

Obiettivo:

Capire se i dati simulati sono compatibili con i parametri teorici.

Studiare la sensibilità della distribuzione ai parametri.