main
{
	var
	    list of int l_nada;
	    int n;
	endvar


    list of int generar(int a){
        var 
            list of int l;
		    list of int primos;
		    list of int fibo;
		    list of int su, mult;
		    int par;
		endvar
            
        list of int producto(list of int l1, list of int l2, list of int l3){
            var
			    list of int prod;
			    int i;
			    int r;
			endvar
			i=0;

			while(i < #l1) {
				r = (l1 @ i) * (l2 @ i) * (l3 @ i);
				prod = prod ++ r @ i;
				i = i+1;
			}

			return prod;
        }
        
        list of int suma(list of int l1, list of int l2, list of int l3){
            var
			    list of int sum;
			    int i;
			    int r;
			endvar
			i=0;

			while(i < #l1) {
				r = (l1 @ i) + (l2 @ i) + (l3 @ i);
				sum = sum ++ r @ i;
				i = i+1;
			}

			return sum;
        }
        
        list of int generarFibonacci(int n3){
            var
                list of int l3;
                int f, f1, f2, i;
            endvar  
                
            if(n3 == 1) {
                l3 = l3++0@0;			            
            }
            else {
                l3 = l3++0@0;
                l3 = l3++1@1;

                i = 2;
                while(i < n) {
                    f1 = l3@(i-1);
                    f2 = l3@(i-2);
                    f = f1 + f2;
                    l3 = l3++f@#l3;

                    i=i+1;
                }
            }
            return l3;
        }
            
        list of int generarPrimos(int n2) {
			var
			    list of int l2;
			    list of int fibo;
			    int j;
			    int c;
			    int pri;
			endvar
            
			pri = 1;
			j = 0;
			while(j < n2) {
				c = 2;
				while(c <= pri-1) {
					if((pri % c) == 0) {
						c = 1000000000;
					}

					c = c+1;
				}

				if(c == pri) {
					l2 = l2 ++ pri @ #l2;
					j = j+1;
				}

				pri = pri+1;
			}

            fibo = generarFibonacci(n2);
			return l2;
		}
    
        par = 0;
        while(par<2*n){
            l = l++par@#l;
            par = par + 2;
        }
        
        primos = generarPrimos(n);
        fibo = generarFibonacci(n);
        mult = producto(l, primos, fibo);
        su = suma(l, primos, fibo);
        cout << "Numeros pares: ";
        cout << l;
        cout << "Numeros primos: ";
        cout << primos;
        cout << "Serie de Fibonacci: ";
        cout << fibo;
        cout << "Suma: ";
        cout << su;
        cout << "Producto: ";
        cout << mult;
        cout << "\n";
        return l;
    }


	cout << "Introduce numero: ";
	cin >> n;

	cout << "\n";

    l_nada = generar(n);
}
