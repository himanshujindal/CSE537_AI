import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.InvalidPropertiesFormatException;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.TreeSet;

/*
 * Author: Himanshu Jindal
 * Description: This program takes a Graph and the probability values from xml files
 * 				and computes the conditional probability of any combination.
 * This was a part of the assignment which i did for AI class
 */

/*Class for storing the Data from A Bayes Net Graph*/
class BayesNetData
{
	/*Nodes to store the data*/
	class Node
	{
		final public Map<Map<String,Boolean>, Float> probs = new HashMap<Map<String,Boolean>, Float>();
		final public Set<String> successors = new TreeSet<String>();
		final public Set<String> predecessors = new TreeSet<String>();
	}
	
	final Map<String, Node> _graph = new HashMap<String, Node>();
	
	private static String NameNormalize(Object o)
	{
		return o.toString().trim().toLowerCase();
	}
	
	/*Loads the graph*/
	private void _loadGraph(InputStream is_graph) throws InvalidPropertiesFormatException, IOException
	{
		final Properties graph = new Properties();
		graph.loadFromXML(is_graph);
		
		for(Map.Entry<Object, Object> item:graph.entrySet())
		{
			final String key = NameNormalize(item.getKey());
			//System.out.println("\nGot key:"+key);
			
			final Set<String> nodes_successor;
			
			if(_graph.containsKey(key))
			{
				nodes_successor = _graph.get(key).successors;
				//System.out.println("Graph already Already contains this key.");
				//System.out.println("Its successors are:"+nodes_successor);
			}
			else
			{
				final Node node = new Node();
				nodes_successor = node.successors;
				_graph.put(key, node);
				//System.out.println("The graph does not have this key. Allocating memory and created this node");
			}
			
			final String[] values = item.getValue().toString().split(",");
			for(String node:values)
			{
				String successor = NameNormalize(node);
				if(!node.isEmpty())
				{
					//System.out.println("Got successor:"+successor);
					
					final Set<String> node_P_predecessors;
					nodes_successor.add(successor);
					//System.out.println("Added this successor to the node. Node successors:"+_graph.get(key).successors);
					//System.out.println("Adding predecessors for the key:"+successor);
					if(_graph.containsKey(successor))
					{
						node_P_predecessors = _graph.get(successor).predecessors;
						//System.out.println("Graph already contains the key "+successor+". Its predecessors are:"+node_P_predecessors);
						
					}
					else
					{
						final Node node_P = new Node();
						node_P_predecessors = node_P.predecessors;
						_graph.put(successor, node_P);
						//System.out.println("Graph does not have the given key. Allocated memory for the key:"+successor);
					}
					node_P_predecessors.add(key);
					//System.out.println("Added predecessor:"+key+". The key:"+successor+" has predecessors:"+_graph.get(successor).predecessors);					
				}
				
			}
		}		
	}
	/*Loads the probabilty table*/
	private void _loadProbs(InputStream is_probs) throws InvalidPropertiesFormatException, IOException
	{
		System.out.println("\nLoading the Probability Table");
		final Properties probs = new Properties();
		probs.loadFromXML(is_probs);
		for(Map.Entry<Object, Object> item:probs.entrySet())
		{
			final String key = NameNormalize(item.getKey().toString());
			//System.out.println("\nKey is: "+key);
			final String[] parts = key.split("\\|");
			final Node node = _graph.get(NameNormalize(parts[0]));
			//System.out.println("Getting node for:"+parts[0]+" with length "+parts.length);
			
			if(node!=null)	// just skip absent nodes
			{
				//System.out.println("Putting prob for "+(parts.length>1?parts[1]:"")+" = "+Float.valueOf(item.getValue().toString()));
				node.probs.put(createProbKey(parts.length>1?parts[1]:""), Float.valueOf(item.getValue().toString()));
			}
		}
	}
	/*Calls the functions to load the Data*/
	public BayesNetData(InputStream is_graph, InputStream is_probs) throws InvalidPropertiesFormatException, IOException
	{
		_loadGraph(is_graph);
		_loadProbs(is_probs);
	}
	
	public Set<String> getNodes() 
	{	// re: actually, we don't need to sort set of nodes. it makes simpler to read.
		return Collections.unmodifiableSet(new TreeSet<String>(_graph.keySet()));
	}
	
	/*Get successors */
	public Set<String> getSuccessors(String snode)
	{
		final Node node = _graph.get(NameNormalize(snode));
		return node==null?null:Collections.unmodifiableSet(node.successors);
	}
	/*
	 * Get predecessors of a given node
	 */
	public Set<String> getPredecessors(String snode)
	{
		final Node node = _graph.get(NameNormalize(snode));
		return node==null?null:Collections.unmodifiableSet(node.predecessors);
	}

	public Map<String, Boolean> createProbKey(String s)
	{
		//System.out.println("Splitting the string "+s+" into ->"+s.split(","));
		return createProbKey(s.split(","));
	}
	
	public Map<String, Boolean> createProbKey(String... skeys)
	{
		final Map<String, Boolean> pkey = new HashMap<String, Boolean>();
		//System.out.println("String skeys is :"+skeys);
		for(String key: skeys)
		{
			boolean value = true;
			String nkey = NameNormalize(key);
			//System.out.println("String nkey is:"+nkey);
			if(nkey.startsWith("~"))
			{
				value = false;
				nkey = NameNormalize(nkey.substring(1));
			}
			if(_graph.containsKey(nkey))
			{
				//System.out.println("Putting value for nkey:"+nkey+" value:"+value);
				pkey.put(nkey, value);
			}
				
		}
		
		//System.out.println("mapkey="+pkey);
		
		return pkey;
	}

	public Map<Map<String,Boolean>, Float> getProbabilityTable(String snode)
	{
		//System.out.println("reached here 4 with snode:"+snode);
		final Node node = _graph.get(NameNormalize(snode));
		return Collections.unmodifiableMap(node.probs);	
	}
	
	/*
	 * Get probability of given node
	 */
	public float getProbability(String node, String cond)
	{
		//System.out.println("Reached here 3 with node:"+node+" cond:"+cond);
		final Map<Map<String,Boolean>, Float> map = getProbabilityTable(node);
		if(map==null)
			return 0;
		final Float v = map.get(createProbKey(cond));
		return v==null?0:v;
	}
	
	/*Makes inference of the data*/
	public float Inference(String node, String cond) 
	{
		if(!cond.isEmpty())
		{
			String numerator = node+","+cond;
			NameNormalize(numerator);
			float den = MakeInference(cond);
			float num = MakeInference(numerator);
			if(den!=0)
			{
				return num/den;
			}
			else
				return 0;
		}
		else
			return MakeInference(node);
	}
	/*Does the inference on each subunit of the data*/
	public float MakeInference(String query)
	{
		System.out.println("\nMaking inference of query:"+query);
		float inf = 0;
		
		/*Getting  the query variables from the query*/
		final String[] queryVar = query.split(",");
		//System.out.println("Split query into:"+queryVar.toString());
		String queryHidden =  "";
		/*Getting the hidden variables from the query*/
		for(String node:getNodes())
		{
			//System.out.println("Node is:"+node);
			boolean node_hidden = true;
			String var_temp = ""; 
			for(String var:queryVar)
			{
				if(var.equals(node) || var.equals("~"+node))
				{
					if(node_hidden == false)
					{
						if(!var.equals(var_temp))
						{
							return 0;
						}
					}
					else
					{
						node_hidden = false;
						var_temp = var;
					}
				}
			}
			if(node_hidden == true)
			{
				//System.out.println("Hidden node found:"+node_hidden);
				if(queryHidden.isEmpty())
					queryHidden+=node;
				else
					queryHidden+=","+node;
			}
		}
		final String[] Hidden_Var = queryHidden.split(",");
		
		/*extending over hidden variables*/ 
		ArrayList<String> query_ext = new ArrayList<String>();
		query_ext.add(query);
		if(Hidden_Var.length>1)
		{
			for(String hid:Hidden_Var)
			{
				int i = query_ext.size();
				while(i>0)
				{
					String query1 = query_ext.get(0);
					query_ext.remove(query1);
					query_ext.add(query1+","+hid);
					query_ext.add(query1+",~"+hid);
					i--;
				}
			}
		}
		for(String query1:query_ext)
		{
			System.out.println("Extended Query 1 :"+query1);
			inf +=SolveQuery(query1);
		}
		
		System.out.println("Result for Query:"+query+" is:"+inf);
		return inf;
	}
	/*code for solving the given query*/
	public float SolveQuery(String Query)
	{
		float inf = 1;
		/*Getting  the query variables from the query*/
		final String[] queryVar = Query.split(",");
		for(String var:queryVar)
		{
			boolean var_val = true;
			//System.out.println("\nVar is:"+var);
			Set <String> predec;
			if(var.charAt(0)=='~')
			{
				var_val = false;
				//System.out.println("Var is false.");
				var = var.substring(1);
				predec = getPredecessors(var);
				//System.out.println("Got predecessors :"+predec);
			}
			else
			{
				//System.out.println("Var is true.");
				predec = getPredecessors(var);
				//System.out.println("Got predecessors :"+predec);
			}
			String cond = "";
			
			if(!predec.isEmpty())
			{
				//System.out.println("Predecessor is not empty and is:"+predec);
				for(String pred:predec)
				{
					if(Query.contains("~"+pred))
					{
						//System.out.println("Query contains pred as false:"+pred);
						if(!cond.isEmpty())
						{
							cond+=",~"+pred;
						}
						else
						{
							cond+="~"+pred;
						}
						//System.out.println("Appended to condition ->"+cond);
					}
					else
					{
						//System.out.println("Query contains pred as true:"+pred);
						if(!cond.isEmpty())
						{
							cond+=","+pred;
						}
						else
						{
							cond+=pred;
						}
						//System.out.println("Appended to condition ->"+cond);
					}	
				}
			}
			//System.out.println("Getting probability for var:"+var+" cond:"+cond);
			float res = getProbability(var, cond);
			if(!var_val)
			{
				res = 1 - res;
			}
			//System.out.println("Result is :"+res);
			inf *= res;
		}
		System.out.println("Result for query->"+Query+" = "+inf);
		return inf;
	}
}

/* This is the main class that prints the graph and probabilty tables and has main function*/
public class BayesNetLoad 
{
	/*Print the graph*/
	public static void PrintGraph(BayesNetData bn)
	{
		System.out.println("\n");
		for(String node: bn.getNodes())
		{
			System.out.println(node+" successors   -> "+bn.getSuccessors(node));
			System.out.println(node+" predecessors -> "+bn.getPredecessors(node));
		}
			
	}
	
	/*Print the Probability table*/
	public static void PrintProbs(BayesNetData bn)
	{
		System.out.println("\n");
		for(String node: bn.getNodes())
		{
			System.out.println(node+" -> "+bn.getProbabilityTable(node));
			//Map<Map<String,Boolean>, Float> map = bn.getProbabilityTable(node);			
		}
	}
	
	public static void main(String[] args) throws IOException
	{	
		//	The paths for the xml files of the graph structure
		//	and the probability tables should be specified here
		final BayesNetData bn = new BayesNetData(new FileInputStream("graph_1.xml"),
											new FileInputStream("probs_1.xml"));
		PrintGraph(bn);	//Print the graph structure
		PrintProbs(bn);	//Print the probability tables
		
		//To take input from user the probability which is to be found
		
		boolean repeat = true;
		while(repeat == true)
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
			
			String node = "";
			String cond = "";
			
			try {
				System.out.println("\n----------------------------------------\n");
				System.out.println("\nThis program finds probability for A|B, where A and B can be any combination of the given variables.");
				System.out.println("\nLeave B empty if there are no conditions.");
				System.out.println("Enter the value of A, the node, separated by ',':");
				node = br.readLine();
				System.out.println("Enter the value of B, the condition, separated by ',':");
				cond = br.readLine();
			}
			catch (IOException ioe) {
				System.out.println("IO error trying to read your name!");
			    System.exit(1);
			}
			if(node.isEmpty())
			{
				System.out.println("Please enter proper values.");
				continue;
			}
			else
			{
				System.out.println("The Probabilistic inference is---->");
				System.out.println("P("+node+"|"+cond+")="+bn.Inference(node, cond));
				System.out.println("Do you want to repeat?");
				try {
					String repeat_value = "";
					repeat_value = br.readLine();
					if(!(repeat_value.contentEquals("yes")|repeat_value.contentEquals("y")|repeat_value.contentEquals("Y")))
					{
						repeat = false;
					}
				}
				catch (IOException ioe) {
					System.out.println("IO error trying to read your name!");
				    System.exit(1);
				}
			}
		}
	}
}