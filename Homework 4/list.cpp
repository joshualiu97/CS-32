void listAll(const Domain* d, string path) // two-parameter overload
{
	if (d == nullptr)
		return;
	if (d->subdomains().empty())
	{
		cout << path << endl;
		return;
	}
	for (size_t i = 0; i < (d->subdomains().size()); i++)
	{
		if (path != "")
			listAll((d->subdomains())[i], (d->subdomains())[i]->label() + "." + path);
		else
			listAll((d->subdomains())[i], (d->subdomains())[i]->label());
	}
	return;
}