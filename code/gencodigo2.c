main
{
	var
		int n, curr, ultim, cuenta, primero;
	endvar
	
	curr = 2;
	ultim = 0;
	cuenta = 0;
	primero = 1;
	
	cout << " introduce numero : ";
	cin >> n;
	cout << " " << n << " == ";
	curr = 2;
	
	while(curr<=n)
	{
	    var 
	        int d;
	    endvar
	    
	    d = n/curr;
	    
	    if(d*curr==n)
	    {
	        if(curr!=ultim)
	        {
	            ultim = curr;
	            cuenta = 1;
	        }
	        else
	            cuenta = cuenta + 1;
	        
	        n = n/curr;
	    }
	    else
	    {
	        if(cuenta>0)
	        {
	            if(primero==0)
	                cout << " *";
	            primero = 0;
	            cout << " " << curr;
	            if(cuenta>1)
	                cout << "^" << cuenta;    
	        }
	        
	        curr = curr+1;
	        cuenta = 0;
	    }
	}
	
	if(cuenta>0)
	{
	    if(primero==0)
	        cout << " *";
	    primero = 0;
	    cout << " " << curr;
	    if(cuenta>1)
	        cout << "^" << cuenta;
	}
	cout << "\n";
}
