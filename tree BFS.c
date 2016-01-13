 void BFS(){
	 std::queue<node*> visited, unvisited; 
	 node nodes[9];
	 node* current;
	 
	 unvisited.push(&nodes[0]); //先把root放入unvisited queue
	 
	 while(!unvisited.empty()) //只有unvisited不空
	 {
	    current = (unvisited.front()); //目前應該檢驗的
	 
	    if(current -> left != NULL)
	       unvisited.push(current -> left);  // 先压入左边的
	 
	    if(current -> right != NULL)         // 再压入右边的
	       unvisited.push(current -> right);
	 
	    visited.push(current);
	 
	    cout << current -> self << endl;
	 
	    unvisited.pop();
	 }
}