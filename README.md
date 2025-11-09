# root-labs

per compilare:
g++ main.cpp mcgenerator.cpp fit_analyzer.cpp functions.cpp `root-config --cflags --libs` -o runMC

per eseguire:
./runMC

SCHEDA 1
pto 1
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
3. fit funzione a parametri fissati partendo dal TGraphErrors del punto 3.2 -> funzione lo trasforma in TH1D -> fit -> calcolo residui (pto 4) tra fit e distribuzione -> si può visualizzare tutto nello stesso canvas in graphs/fit_from_graph.png 
4. considero davvero grafico generato nel punto 3.2 (con incertezza da rigenerazione e con parametri k, b, teta senza incertezza)
-> fit di quello a parametri fissi + calcolare resido (cambiera sempre al variare di N_replicas)

da finire pto 5 ascheda 2
variando parametri fit (parametri con incertezza credo) vedi come varia residuo, questo punto l'ho inteso come un dare per fissato solo due dei parametri b, teta e k e a turno variarne uno *ad esempio estraendone un valore random tra piu e meno uno il valore fisso/usando le incertezze della scheda uno e vedere come variano il fit e residuo
