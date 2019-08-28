void removeBad(list<Movie*>& li)
{
	list<Movie*>::iterator p = li.end();
	while (p != li.begin())
	{
		p--;
		if ((*p)->rating() < 55)
		{
			delete *p;
			p = li.erase(p);
		}
	}
}