# root-labs

nel metodo drawfunction l'obbiettivo è generare dopo aver generato un istogramma con MC sovrapporlo alla funzione teorica-> devono avere la stessa scala (stessa area sotto la curvatura)
1. è normalizzato l'istogramma a 1
2. creo funzione teorica f_norm
3. calcolo area sotto la funzione teorica (double integral_f = f_norm.Integral(x_min, x_max);) -> così conosco il peso della funzione nel range scelto
4. double scale_factor = integral_h / integral_f; questo è il coefficiente moltiplicativo x rendere la funzione teorica confrontabile con l'istogramma (se la funzione ha area 2 e l’istogramma ha area 1, il fattore sarà 0.5 → abbassa la funzione)
5. creo nuova funzione e la scalo per il fattore di scala appena ottenuto
6. disegno h e f_scaled nello stesso canvas


da fare
- aggiungere alla "legenda" lo scarto quadratico medio