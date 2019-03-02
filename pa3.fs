/// Megan Nguyen
/// I had composed the code I wrote based on my 
/// understanding of how the features of the 
/// language I am using can be used to implement
/// the algorithm I had chosen to solve the problem
/// I am addressing.
/// 
open System

///_____________PART 1______________________________

/// Function takes a list of tuples as an argument.
/// Function makes the assumption that each tuple in 
/// the list is of the type (float, float, float), 
/// which represent the 3 dimension of a cube. Function
/// calculates the volume of each cube and returns the 
/// value with the largest volume
let rec maxCubeVolume = function
    | [] -> 0.0         //if list empty then return max volume as 0.0
    | (l, w, h) ::tl -> //otherwise extract the head tuple from the rest of the list
        let max = maxCubeVolume tl //recursively look for the max volume from the rest of the list
        let t_volume = l*w*h //calculate volume for the head cube
        if max >= t_volume then max //compare the volume of head and rest of list, return the larger  cube
        else t_volume   

///_____________PART 2______________________________

/// Function takes a string (search target) and a 
/// list of tuples as arguments. Function makes an
/// assumption that each tuple in the list is of the
/// type (string, int). Function searchs through list
/// and find all tuples with their string component
/// that matches with the search target, and return a 
/// sorted list of int component of the matched tuples
let rec findMatches s = function
    | [] -> []      //if list empty then return empty list
    | (k,v)::tl ->  //otherwise extract the head tuple from the rest of list
        //compare string of tuple with target search,
        //if match, then link int of tuple with the 
        //rest of result (by call findMatches recursively on the rest of the list)
        //otherwise look for match from the rest of list
        if k = s then List.sort (v::findMatches s tl) 
        else List.sort (findMatches s tl)


///___________________PART 3________________________

/// Given Type definition for BST
type BST =
    | Empty
    | TreeNode of int * BST * BST

/// Function takes 2 arguments, a value to be inserted
/// and a tree to inserted into. If the value already 
/// exist in the tree, then the original tree is returned
let rec insert value tree =
    match tree with
    //if tree is empty then return a new tree with value as root and no left, right subtree
	| Empty -> TreeNode (value, Empty, Empty)
	| TreeNode (rt, left, right) -> //otherwise extract the component of the tree
        //compare value with root, if less than root,
        //then recursively insert value to the left subtree
        //if greater than root then insert to the right subtree
        //else (equal to root) then just return original tree
		if rt > value then TreeNode (rt, insert value left, right)
		else if rt < value then TreeNode (rt, left, insert value right)
        else TreeNode (rt, left, right)

/// Function take 2 arguments, a value to be compared to
/// and a tree to check for. Function search tree recursively 
/// If the value is in the tree, then return true, 
/// otherwise return false. 
let rec contains value tree =
    match tree with
    | Empty -> false    //if tree is empty then return false
    | TreeNode (rt, left, right) -> //otherwise extract the component of the tree
        if rt = value then true //if root = to value then return true, done
        else //otherwise
            let result_left = contains value left //look for value in the left subtree
            if result_left = true then result_left //if left subtree return true then done
            else contains value right //otherwise look in the right subtree

///Function takes 2 arguments, a function that evaluate a given
/// value and return true or false, and a tree with nodes to be
/// evaluated for. Function returns the number of nodes in the tree
/// that is evaluated to true
let rec count func tree =
    match tree with
	| Empty -> 0 //if tree is empty, not to evaluate for, return 0, done
	| TreeNode (rt, left, right) -> //otherwise extract the components of the tree
		//if root value is evaluated to true then add 1 to the result of 
        //count for the left and right subtree (recursively)
        //otherwise just return the count for left and right subtree
        if func rt = true then 1 + count func left + count func right
		else count func left + count func right

/// Function takes 1 arguments, a tree to be counted for its even nodes. 
/// Function uses the help of count function to find the number of even nodes
/// Function returns the number of even nodes in the tree
let evenCount tree =
	let even x = x%2=0 //define a lamda function that return true if value divisible by 2, false otherwise
	count even tree //call count function to help evaluate and count the number of even nodes