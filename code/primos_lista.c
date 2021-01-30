main
{
	var
	    list of int l_nada;
	    int n;
	endvar

	list of int generarFibo(int n) {
		var
		    list of int l;
		    list of int l_primos;
		    list of int l_fibo;
		    list of int l_suma;
		    list of int l_mul;
		    int i;
		    int f;
		    int f_prev_1;
		    int f_prev_2;
		endvar

		list of int generarPrimos(int n) {
			var
			    list of int l2;
			    int count;
			    int c;
			    int i;
			endvar

			i = 3;
			count = 2;
			n = n+1;
			while(count <= n) {
				c = 2;
				while(c <= i-1) {
					if((i % c) == 0) {
						c = 1000000000;
					}

					c = c+1;
				}

				if(c == i) {
					l2 = l2 ++ i @ #l2;
					count = count+1;
				}

				i = i+1;
			}

			return l2;
		}

		list of int sumar(list of int l1, list of int l2) {
			var
			    list of int l;
			    int i;
			    int r;
			endvar
			i=0;

			while(i < #l1) {
				r = (l1 @ i) + (l2 @ i);
				l = l ++ r @ i;
				i = i+1;
			}

			return l;
		}

		list of int mul(list of int l1, list of int l2) {
			var
			    list of int l;
			    int i;
			    int r;
			endvar
			i=0;

			while(i < #l1) {
				r = (l1 @ i) * (l2 @ i);
				l = l ++ r @ i;
				i = i+1;
			}

			return l;
		}


		if(n == 1) {
			l = l ++ 1 @ 0;
		}
		else if(n == 2) {
			l = l ++ 1 @ 0;
			l = l ++ 1 @ 0;
		}
		else {
			l = l ++ 1 @ 0;
			l = l ++ 1 @ 0;

			i = 2;
			while(i < n) {
				f_prev_1 = l @ (i-1);
				f_prev_2 = l @ (i-2);
				f = f_prev_1 + f_prev_2;
				l = l ++ f @ #l;

				i = i+1;
			}
		}

		l_primos = generarPrimos(n);
		l_fibo   = l;
		l_suma   = sumar(l_primos, l_fibo);
		l_mul    = mul(l_primos, l_fibo);


		cout << "Fibonacci: ";
		cout << l_fibo;
		cout << "Primos: ";
		cout << l_primos;
		cout << "Suma: ";
		cout << l_suma;
		cout << "Multiplicacion:  "; 
		cout << l_mul;

		return l;
	}



	cout << "Introduce numero: ";
	cin >> n;

	cout << "\n";

	l_nada = generarFibo(n);

}
