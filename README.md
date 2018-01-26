Part 1: Graph structure-based Search Engine

The pagerank.c file reads data from a given collection of pages in the file collection.txt and builds a graph structure using an Adjacency Matrix. Using the algorithm described below, we calculated the Weighted PageRank for every url in the file collection.txt. In this file, urls are separated by one or more spaces or/and new line character. Added suffix '.txt' to a url to obtain file name of the corresponding "web page". For example, file url24.txt contains the required information for url24. The algorithm to update PageRank values is given by:

	PageRankW(d, diffPR, maxIterations){

		Read "web pages" from the collection in file "collection.txt" and build a graph structure using Adjacency List Representation

		N = number of urls in the collection

		For each url p_i in the collection
            
>>>>>>>![equation](https://latex.codecogs.com/gif.latex?PR(p_i;0)&space;=&space;\frac{1}{N})
   
		End For

		iteration = 0;
       	diff = diffPR;   // to enter the following loop

        While (iteration < maxIteration AND diff >= diffPR)
        	iteration++;
>>>>>>>>![equation](https://latex.codecogs.com/gif.latex?PR(p_i;t&plus;1)&space;=&space;\frac{1-d}{N}&plus;d\sum_{p_j\epsilon&space;M(p_i)}^{&space;}PR(p_j;t)*W_{(p_j,&space;p_i)}^{in}*W_{(p_j,&space;p_i)}^{out})
   
>>>>>>>>![equation](https://latex.codecogs.com/gif.latex?diff&space;=&space;\sum_{i=1}^{N}|PR(p_i;&space;t&plus;1)-PR(p_i;&space;t)|)
   
    	End While
   


