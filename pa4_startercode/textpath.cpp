
    bool firstIter = true;

    int visitedLastTimes = 0;

    while (!q.empty()) {
        // pop first and then get children (set distance and pred) then push to q
        string currentAct = q.front();
        q.pop();
       
        // find the first actor in our map, is our start vertex
        MapAddr::iterator addrIt = addrMap.find(currentAct);
        ActorNode* decoy1 = addrIt->second;
        UmapNodes::iterator mapIt = theMap.find(decoy1);

        cout << "~~~Popping!"<< endl;
        cout << "ActorName in Pathfinder: " << currentAct << endl;
        cout << "distance if any: " << mapIt->first->distance << endl;
        
        
        if (firstIter == true) { // first iteration, set dist to 0
            
            ActorNode* firstIterNode = mapIt->first;
            EdgeSet edges = mapIt->second;
            firstIterNode->distance = 0;
            for(EdgeSet::iterator it = edges.begin(); it != edges.end(); ++it) {
                // push first node's children, after making their pred the first node
                ActorEdge kid = *it;
                string kidName = kid.getPartner();
                MapAddr::iterator aIt = addrMap.find(kidName);
                ActorNode* kidNode = aIt->second;
                kidNode->distance = 1;
                kidNode->pred = firstIterNode;
                kidNode->moviePath = kid;
                q.push(kidName);
            }

            firstIter = false;
        }
        // normal iterations
        else {
        
            ActorNode* theNode = mapIt->first;
            EdgeSet nodeE = mapIt->second;
            for (EdgeSet::iterator itSet = nodeE.begin(); itSet != nodeE.end(); ++itSet) {
                // iterating through all edges (adj nodes)
                ActorEdge shid = *itSet;
                string shidName = shid.getPartner();
                MapAddr::iterator bIt = addrMap.find(shidName);
                ActorNode* shidNode = bIt->second;
                // if not predecessor
