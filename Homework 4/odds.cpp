void removeOdds(vector<int>& v)
{
	vector<int>::iterator p = v.end();
	while (p != v.begin())
	{
		p--;
		if (*p % 2 == 1)
			p = v.erase(p);
	}
}