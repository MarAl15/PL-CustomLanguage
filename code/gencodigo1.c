main
{
	var
		int n, curr;
	endvar
	
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
	        cout << "* " << curr << " ";
	        n = n/curr;
	    }
	    else
	        curr = curr+1;
	}
	cout << "\n";
}
