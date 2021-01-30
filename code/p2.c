
main
{
	var
		i·nt ve;
		int ve1, v2;
		int vf;
		char vc;
		bool vl;
		list of int pe, pe2;
		list of float pf, pf2;
		list of char pc, pc2;
		list of bool pl;
	endvar
	int funcionA (int a1, float a2, char a3)
	{
		var
			int x1, x2,a2;
		endvar
		char funcionB (char b1, bool b2)
		{
			var
				float x1, x2;
				int v1;
				list of char c;
				list of int e,b1;
				bool prub;
			endvar
            
			float funcionC (bool c1, int c2)
			{
				var
					float x1;
				endvar

				x1 = +-+-+-+1.3;
				ve1 = ve*2;
				ve1= funcionA(2+3,2.5,'a');
				vf = funcionC(2,4,6);
				x1 = funcionC(false, 5);

				if ((c2>10)+2)
					c2 = c2+1;
				else
					x1 = 3.1;
				return x1;
			}

			x1=funcionC(true, 10);
			x2=x1*(funcionC(false,1)-funcionC(true,23))/10.0;
			c = c/5;
			while(x2*funcionC(false,1)-x1<10.0)
				x2= x2*x1;
				
		    return 'a';
		}

		float funcionD (float d1)
		{
			var
				char dato;
				int valor;
			endvar
			char funcionE(char e1, char e2)
			{
				cout << "introduzca dos caracteres: ";
				cin >> e1, e2;
				if (e1=='a')
					return e1;
				else if (e1=='b')
					return e2;
				else
					return 'a';
			}
            
			cout << "introduzca un valor entero";
			cin >> valor;

			if(d1>0.0)
			{
				var
					int dato2;
				endvar
				
				dato2 =2;
				dato2 = valor*20/dato2;
			}
			else{
				valor=valor*100;
				d1=d1/1000.0;
			}
			
			return d1;
		}
		
		list of char funcionF(list of char lista){
			var
				list of char aux;
				char elemento;
				int posicion;
			endvar
			
			lista = ['a', 'b'];
			elemento = 'a';
			posicion = 3;
			aux = lista++elemento@posicion;
			
			lista = lista ** ['c', 'd'];
			lista<<;
			return aux;
		}
        
		return 1;
	}
	pe = pe*ve1;
	pe = pe -vf;
	pf = pc+1;
	cout << "fin";
}
