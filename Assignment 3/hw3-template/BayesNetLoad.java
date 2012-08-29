import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
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
		final public Set<String> neighbours = new TreeSet<String>();
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
			
			final Set<String> nodes;
			
			if(_graph.containsKey(key))
				nodes = _graph.get(key).neighbours;
			else
			{
				final Node node = new Node();
				nodes = node.neighbours;
				_graph.put(key, node);
			}
			
			final String[] values = item.getValue().toString().split(",");
			for(String node:values)
				nodes.add(NameNormalize(node));
		}		
	}
	/*Loads the probabilty table*/
	private void _loadProbs(InputStream is_probs) throws InvalidPropertiesFormatException, IOException
	{
		final Properties probs = new Properties();
		probs.loadFromXML(is_probs);
		for(Map.Entry<Object, Object> item:probs.entrySet())
		{
			final String key = item.getKey().toString();
			final String[] parts = key.split("\\|");
			final Node node = _graph.get(NameNormalize(parts[0]));
			if(node!=null)	// just skip absent nodes
				node.probs.put(createProbKey(parts.length>1?parts[1]:""), Float.valueOf(item.getValue().toString()));
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
	
	/*function to get the neighbour of the nodes*/
	public Set<String> getNeighbours(String snode)
	{
		final Node node = _graph.get(NameNormalize(snode));
		return node==null?null:Collections.unmodifiableSet(node.neighbours);
	}
	
	/*To create the probabilty key*/
	public Map<String, Boolean> createProbKey(String s)
	{
		return createProbKey(s.split(","));
	}
	
	public Map<String, Boolean> createProbKey(String... skeys)
	{
		final Map<String, Boolean> pkey = new HashMap<String, Boolean>();
		
		for(String key: skeys)
		{
			boolean value = true;
			String nkey = NameNormalize(key);
			if(nkey.startsWith("~"))
			{
				value = false;
				nkey = NameNormalize(nkey.substring(1));
			}
			if(_graph.containsKey(nkey))
				pkey.put(nkey, value);
		}
		
		System.out.println("mapkey="+pkey);
		
		return pkey;
	}

	public Map<Map<String,Boolean>, Float> getProbabilityTable(String snode)
	{
		final Node node = _graph.get(NameNormalize(snode));
		return Collections.unmodifiableMap(node.probs);	
	}
	
	public float getProbability(String node, String cond)
	{
		final Map<Map<String,Boolean>, Float> map = getProbabilityTable(node);
		if(map==null)
			return 0;
		final Float v = map.get(createProbKey(cond));
		return v==null?0:v;
	}
}

/* This loads the Bayes net and does the computation for finding probabilty*/
public class BayesNetLoad 
{
	public static void PrintGraph(BayesNetData bn)
	{
		for(String node: bn.getNodes())
			System.out.println(node+" -> "+bn.getNeighbours(node));
	}
	/*To print the probabilties of the nodes*/
	public static void PrintProbs(BayesNetData bn)
	{
		for(String node: bn.getNodes())
		{
			System.out.println(node+" -> "+bn.getProbabilityTable(node));
			//Map<Map<String,Boolean>, Float> map = bn.getProbabilityTable(node);			
		}
	}
	
	public static void main(String[] args) throws IOException
	{	
		final BayesNetData bn = new BayesNetData(new FileInputStream("y:/graph.xml"),
											new FileInputStream("y:/probs.xml"));
		PrintGraph(bn);
		final String node = "grass_wet";
		final String cond = "rain,~sprinkler";
		final String condequ = "~sprinkler,rain";
		System.out.println("Prob tables:");
		PrintProbs(bn);
		System.out.println("-------------------------------");
		System.out.println("P("+node+"|"+cond+")="+bn.getProbability(node, cond));
		System.out.println("P("+node+"|"+condequ+")="+bn.getProbability(node, condequ));
	}
}
