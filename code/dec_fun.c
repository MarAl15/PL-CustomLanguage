extern 	int n;
extern 	lista l_nada;

lista generar (int a)
{
	int par;
	lista fibo;
	lista primos;
	lista l_par;
	lista l;

	lista_ini(&fibo,1);
	lista_ini(&fibo,1);
	lista_ini(&fibo,1);
	lista_ini(&fibo,1);

	lista producto (lista l1, lista l2, lista l3)
	{
		int r;
		int i;
		lista prod;

		lista_ini(&prod,1);

		{
			i = 0;
		}
		{
			etiqueta0:;
			int temp0 = longitud(&l1);
			bool temp1 = i < temp0;
			if (!temp1) goto etiqueta1;
			{
				{
					int temp2 = getI(&l1,i);
					int temp3 = getI(&l2,i);
					int temp4 = temp2 * temp3;
					int temp5 = getI(&l3,i);
					int temp6 = temp4 * temp5;
					r = temp6;
				}
				lista temp7 = setI(&prod,r,i);
				prod = temp7;
				int temp8 = i + 1;
				i = temp8;
			}
			goto etiqueta0;
			etiqueta1:;
		}
		return prod;
	}
	lista suma (lista l1, lista l2, lista l3)
	{
		int r;
		int i;
		lista sum;

		lista_ini(&sum,1);

		{
			i = 0;
		}
		{
			etiqueta2:;
			int temp9 = longitud(&l1);
			bool temp10 = i < temp9;
			if (!temp10) goto etiqueta3;
			{
				{
					int temp11 = getI(&l1,i);
					int temp12 = getI(&l2,i);
					int temp13 = temp11 + temp12;
					int temp14 = getI(&l3,i);
					int temp15 = temp13 + temp14;
					r = temp15;
				}
				lista temp16 = setI(&sum,r,i);
				sum = temp16;
				int temp17 = i + 1;
				i = temp17;
			}
			goto etiqueta2;
			etiqueta3:;
		}
		return sum;
	}
	lista generarFibonacci (int n3, lista pares)
	{
		int i;
		int f2;
		int f1;
		int f;
		lista l3;

		lista_ini(&l3,1);

		lista generarPrimos (int n2, lista lp, lista lf)
		{
			lista mult;
			lista su;
			int pri;
			int c;
			int j;
			lista l2;

			lista_ini(&su,1);
			lista_ini(&su,1);
			lista_ini(&su,1);

			{
				pri = 1;
			}
			j = 0;
			{
				etiqueta4:;
				bool temp18 = j < n2;
				if (!temp18) goto etiqueta5;
				{
					{
						c = 2;
					}
					{
						etiqueta6:;
						int temp19 = pri - 1;
						bool temp20 = c <= temp19;
						if (!temp20) goto etiqueta7;
						{
							{
								int temp21 = pri % c;
								bool temp22 = temp21 == 0;
								{
									if (!temp22) goto etiqueta8;
									{
										{
											c = 1000000000;
										}
									}
								}
								etiqueta8:;
							}
							int temp23 = c + 1;
							c = temp23;
						}
						goto etiqueta6;
						etiqueta7:;
					}
					bool temp24 = c == pri;
					{
						if (!temp24) goto etiqueta10;
						{
							{
								int temp25 = longitud(&l2);
								lista temp26 = setI(&l2,pri,temp25);
								l2 = temp26;
							}
							int temp27 = j + 1;
							j = temp27;
						}
					}
					etiqueta10:;
					int temp28 = pri + 1;
					pri = temp28;
				}
				goto etiqueta4;
				etiqueta5:;
			}
			su = suma(lp, l2, lf);
			mult = producto(lp, l2, lf);
			printf("%s", "Numeros pares: ");
			imprimir(&lp);
			printf("%s", "Numeros primos: ");
			imprimir(&l2);
			printf("%s", "Serie de Fibonacci: ");
			imprimir(&lf);
			printf("%s", "Suma: ");
			imprimir(&su);
			printf("%s", "Producto: ");
			imprimir(&mult);
			printf("%s", "\n");
			return l;
			imprimir(&lp);
			return l2;
		}
		{
			bool temp29 = n3 == 1;
			{
				if (!temp29) goto etiqueta12;
				{
					{
						lista temp30 = setI(&l3,0,0);
						l3 = temp30;
					}
				}
				goto etiqueta13;
				etiqueta12:;
				{
					{
						lista temp31 = setI(&l3,0,0);
						l3 = temp31;
					}
					lista temp32 = setI(&l3,1,1);
					l3 = temp32;
					i = 2;
					{
						etiqueta14:;
						bool temp33 = i < n;
						if (!temp33) goto etiqueta15;
						{
							{
								int temp34 = i - 1;
								int temp35 = getI(&l3,temp34);
								f1 = temp35;
							}
							int temp36 = i - 2;
							int temp37 = getI(&l3,temp36);
							f2 = temp37;
							int temp38 = f1 + f2;
							f = temp38;
							int temp39 = longitud(&l3);
							lista temp40 = setI(&l3,f,temp39);
							l3 = temp40;
							int temp41 = i + 1;
							i = temp41;
						}
						goto etiqueta14;
						etiqueta15:;
					}
				}
			}
			etiqueta13:;
		}
		l3 = generarPrimos(n3, pares, l3);
		return l3;
	}
	{
		par = 0;
	}
	{
		etiqueta16:;
		int temp42 = 2 * n;
		bool temp43 = par < temp42;
		if (!temp43) goto etiqueta17;
		{
			{
				int temp44 = longitud(&l_par);
				lista temp45 = setI(&l_par,par,temp44);
				l_par = temp45;
			}
			int temp46 = par + 2;
			par = temp46;
		}
		goto etiqueta16;
		etiqueta17:;
	}
	fibo = generarFibonacci(n, l_par);
	return l;
}
