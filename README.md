

## Part 1: Graph structure-based Search Engine

The pagerank.c file reads data from a given collection of pages in the file collection.txt and builds a graph structure using an Adjacency Matrix. Using the algorithm described below, we can calculate the Weighted PageRank for every url in the file collection.txt. In this file, urls are separated by one or more spaces or/and new line character. Added suffix '.txt' to a url to obtain file name of the corresponding "web page". For example, file url24.txt contains the required information for url24. The algorithm to update PageRank values is given below.

 ***Definitions***

 

 ![equation](https://latex.codecogs.com/gif.latex?M(p_i))
 
 Set containing nodes (urls) with outgoing links to ![equation](https://latex.codecogs.com/gif.latex?p_i) (ignore self-loops and parallel edges)
 
 ![equation](https://latex.codecogs.com/gif.latex?W_{(v,&space;u)}^{in}&space;=&space;\frac{I_u}{\sum_{p\epsilon&space;R(v)}^{&space;}I_p}) 
 
 Weight of *link(v,u)* calculated based on the number of inlinks of page u and the number of inlinks of all reference pages of page v.

![equation](https://latex.codecogs.com/gif.latex?I_u,&space;I_p)

Represent the number of inlinks of page u
and page p, respectively.
 
 ![equation](https://latex.codecogs.com/gif.latex?W_{(v,&space;u)}^{out}&space;=&space;\frac{O_u}{\sum_{p\epsilon&space;R(v)}^{&space;}O_p}) 
 
Weight of *link(v, u)* calculated based on the number of outlinks of page u and the number of outlinks of all reference pages of page v.

![equation](https://latex.codecogs.com/gif.latex?O_u,&space;O_p)

Represent the number of outlinks of page u and page p, respectively.

![equation](https://latex.codecogs.com/gif.latex?R(v))

Denotes the reference page list of page v.

![equation](https://latex.codecogs.com/gif.latex?t,&space;(t&plus;1))

Correspond to values of iterations.

Refer to [Weighted PageRank Algorithm](https://www.cse.unsw.edu.au/~cs2521/17s2/ass/ass02/page1725348.pdf) for more information.
 
  ***Assumptions***

For calculating  ![equation](https://latex.codecogs.com/gif.latex?W_{(p_j,&space;p_i)}^{out}&space;=&space;\frac{O_u}{\sum_{p\epsilon&space;R(v)}^{&space;}O_p}) , if a node *k* has 0 out degree (zero outlink), ![equation](https://latex.codecogs.com/gif.latex?O_k) should be 0.5 and not 0. This will aviod issues related to division by 0. 
 
 ***Algorithm for PageRank Calculation***

	Start PageRankW(d, diffPR, maxIterations)

		Read "web pages" from the collection in file "collection.txt" and build a graph structure using Adjacency List Representation

		N = number of urls in the collection

		For each url p_i in the collection
            
![equation](https://latex.codecogs.com/gif.latex?PR(p_i;0)&space;=&space;\frac{1}{N})
   
		End For 

		iteration = 0;
       	diff = diffPR;   // to enter the following loop

        While (iteration < maxIteration AND diff >= diffPR)
        	iteration++;
![equation](https://latex.codecogs.com/gif.latex?PR(p_i;t&plus;1)&space;=&space;\frac{1-d}{N}&plus;d\sum_{p_j\epsilon&space;M(p_i)}^{&space;}PR(p_j;t)*W_{(p_j,&space;p_i)}^{in}*W_{(p_j,&space;p_i)}^{out})
   
![equation](https://latex.codecogs.com/gif.latex?diff&space;=&space;\sum_{i=1}^{N}|PR(p_i;&space;t&plus;1)-PR(p_i;&space;t)|)
   
    	End While
    	
    End PageRankW(d, diffPR, maxIterations)


----------


## Part-2: Content-based Search Engine

The file searchTfIdf.c receives search terms (words) as command-line arguments and outputs (to stdout) top 30 pages in descending order of number of search terms found and then within each group, arranges search terms in descending order of summation of tf-idf values. The program also outputs the corresponding summation of tf-idf along with each page, separated by a space and using format "%.6f". See example below.

 ***Example***

    % searchTfIdf  mars  design
        url25  1.902350
        url31  0.434000
 ***Term Frequency Calculation***
 
 The term frequency is given by 

![equation](https://latex.codecogs.com/gif.latex?tf(t,d)&space;=&space;\frac{F_t\epsilon_d}{w_d})
where ![equation](https://latex.codecogs.com/gif.latex?F_t\epsilon_d) is the frequency of term *t* in document *d* and ![equation](https://latex.codecogs.com/gif.latex?{w_d}) is the total number of words in *d*. 

 ***Inverse Document Frequency Calculation***

The inverse document frequency is given by 

![equation](https://latex.codecogs.com/gif.latex?idf(word,D)&space;=\log&space;\frac{N}{|{d\epsilon&space;D:t\epsilon&space;d|}})
where ![equation](https://latex.codecogs.com/gif.latex?N) is the total number of documents and ![equation](https://latex.codecogs.com/gif.latex?D) is the set of all documents.  


Refer to [tf-idf ](https://en.wikipedia.org/wiki/Tf-idf) for more information on these calculations. 


----------


## Part-3: Hybrid/Meta Search Engine using Rank Aggregation

Here, we combine search results (ranks) from Part 1 and Part 2 using the "Scaled Footrule Rank Aggregation" method. Let T1 and T2 be the search results (ranks) obtained using Part 1 and Part 2 respectively.

Then, a weighted bipartite graph for scaled footrule optimization (C,P,W) is defined as follows: 

 -  Let 'C' be the set of nodes to be ranked (union of T1 and T2). 
 - Let P be the set of positions available (say {1, 2, 3, 4, 5}).
 
 Then, W(c,p) is the scaled-footrule distance (from T1 and T2) of a ranking that places element 'c' at position 'p', given by: 

![equation](https://latex.codecogs.com/gif.latex?W(c,p)&space;=&space;\sum_{i=1}^{k}|\tau_i(c)/|\tau_i|-\frac{p}{n}|)
where

 - ![equation](https://latex.codecogs.com/gif.latex?n=|C|)  
 - |T1| is the cardinality (size) of T1
 - |T2| is the cardinality (size) of T2
 - ![equation](https://latex.codecogs.com/gif.latex?T_1(x_3)) is the position of ![equation](https://latex.codecogs.com/gif.latex?x_3) in ![equation](https://latex.codecogs.com/gif.latex?T_1)
 - k is the number of rank lists

 ***Example***

![enter image description here](https://www.cse.unsw.edu.au/~cs2521/17s2/ass/ass02/scaled-footrule-example1.png)
 
The final ranking is derived by finding possible values of position 'P' such that the scaled-footrule distance is minimum. There are many different ways to assign possible values for 'P'. In the above example P = [1, 3, 2, 5, 4]. Some other possible values are, P = [1, 2, 4, 3, 5], P = [5, 2, 1, 4, 3], P = [1, 2, 3, 4, 5], etc. For n = 5, possible alternatives are 5!. For n = 10, possible alternatives would be 10! - that is 3,628,800 alternatives. A very simple and obviously inefficient approach could use brute-force search and generate all possible alternatives, calculate scaled-footrule distance for each alternative, and find the alternative with minimum scaled-footrule distance.

 


----------
