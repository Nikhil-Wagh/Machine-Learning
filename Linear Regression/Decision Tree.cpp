#include<bits/stdc++.h>
#include<set>
using namespace std;
int m;
typedef struct node{
    int n, feature;
    map <int, node* > children;
    set <int> used_features;
    vector < vector<int> > dataset;
    float entropy;
    int value;
}node;
float get_entropy(map <int, int> m)                                 //Calculates the entropy E(s) = -(p+)*log(p+) - (p-)*log(p-)
{
    float entropy = 0;
    map <int, int> :: iterator it;
    float total=0;
    for (it = m.begin();it != m.end();it++)
        total += it->second;
    for (it = m.begin();it != m.end();it++)
    {
        float value = ((float)it->second)/total;
        entropy += value*log(value);
    }
    return (-entropy/log(2));
}
float Entropy(node *temp, int feature)                              //Calculates entropy for a feature E(T,X) = sum(P(c)*E(c))
{
    float entropy = 0;
    int n = temp->n;
    set <int> distinct_values;
    set <int> :: iterator it;
    if(feature < m)
    {
        for (size_t i=0; i<n; i++)
        {
            distinct_values.insert(temp->dataset[i][feature]);
        }
        for (it = distinct_values.begin(); it != distinct_values.end(); it++)
        {
            map <int, int> freq_table;
            int total = 0;
            for(size_t j=0; j<n; j++)
            {
                if(temp->dataset[j][feature] == *it)
                {
                    freq_table[temp->dataset[j][m]]++;
//                    cout<<j+1<<" "<<temp->dataset[j][m]<<" "<<freq_table[temp->dataset[j][m]]<<endl; //something wrong here.
                    total++;
                }
            }
            entropy += ((float)total/(float)n)*get_entropy(freq_table);
            //cout<<feature<<" "<< *it <<" "<<"Entropy: "<<entropy<<endl;
        }
        return entropy;
    }
    else
        cout<<"Something went Wrong!"<<endl;
    return -1;
}
void create_tree(node *temp)
{
    set <int> distinct_targets;
    int n = temp->n;
    for (size_t i=0; i<n; i++)
    {
        distinct_targets.insert(temp->dataset[i][m]);
    }
//    cout<<"Distinct Targets: "<<distinct_targets.size()<<" "<<n<<endl;
//    set <int> :: iterator it;
//    for(it=distinct_targets.begin();it != distinct_targets.end(); it++)
//        cout<<*it<<" ";
//    cout<<endl;
    if(distinct_targets.size()<=1 || temp->used_features.size() == m)// stopping condition if all targets are same or all features are exhausted.
    {
        node *leaf = new node;
        leaf->feature = -1;
        leaf->value = *(distinct_targets.begin());
        temp->children[leaf->value] = leaf;
        cout<<"Leaf Value: "<<leaf->value<<endl;
    }
    else
    {
        float max_gain = -1;
        size_t i, j, selected_feature=-1;
        for(i=0;i<m;i++)
        {
            if(temp->used_features.find(i) == temp->used_features.end())
            {
                float gain = temp->entropy - Entropy(temp, i);
    //            cout<<gain<<endl<<endl;                                     // Gain working properly
                if(max_gain < gain)
                {
                    max_gain = gain;
                    selected_feature = i;
                }
            }
        }
        temp->feature = selected_feature;
        set<int> distinct_values;
        for(i=0;i<n;i++)
            distinct_values.insert(temp->dataset[i][selected_feature]); // no of children = distinct_values.size()
        set<int> :: iterator it;
        int child_count = 0;
        for(it = distinct_values.begin(); it != distinct_values.end(); it++) // new child
        {
            node *child = new node;
            // dataset of this child

//            child->children;
//            child->dataset;           done
//            child->entropy;           done
//            child->feature;           done
//            child->n;                 done
//            child->no_of_features;    done
//            child->value;
//            child->used_feature
            int n_child = 0;
            child->used_features = temp->used_features;
            child->used_features.insert(selected_feature);
            child->value = child_count++;
            for(i=0;i<n;i++)
            {
                if(*it == temp->dataset[i][selected_feature])
                {
                    vector <int> row;
                    for(j=0;j<m+1;j++)
                    {
                        if(j != selected_feature)
                        {
                            row.push_back(temp->dataset[i][j]);
                        }
                        else
                            row.push_back(-1);
                    }
                    child->dataset.push_back(row);
                    n_child++;
                }
            }
            child->n = n_child;
            child->feature = selected_feature;
            cout<<"Feature: "<<selected_feature<<" "<<"Value: "<<child->value<<endl;
            cout<<"Child Dataset:"<<endl;
            for(i=0;i<child->n;i++)
            {
                for(j=0;j<m+1;j++)
                    cout<<child->dataset[i][j]<<" ";
                cout<<endl;
            }
            cout<<endl;
            //entropy of this child
            child->entropy = Entropy(temp, selected_feature);
            temp->children[child->value] = child;
            create_tree(child);
        }
    }
}
int traverse(node *temp, vector<int> v)
{
    if(temp->feature == -1)
        return temp->value;
    int value = v[temp->feature];
    //cout<<temp->feature<<" "<<temp->value<<endl;
    return traverse(temp->children[value], v);
}
int main()
{
    int i, j;
    node *head = new node;
    int n;
    cin >> n >> m;
    head->n = n;
    head->entropy = 0.94;
    list <int> available_features;
    map<int, int> test;
   /* test[0] = 5;
    test[1] = 9;                  //  get_entropy() working fine.
    cout<<"E(5,9): "<<get_entropy(test);*/
    for (i=0;i<n;i++)
    {
        vector <int> row;
        float x;
        for (j=0;j<m + 1;j++)
        {
            cin>>x;
            row.push_back(round(x));
        }
        head->dataset.push_back(row);
    }
    create_tree(head);
    char ch;
    do{
        cout<<"Enter example to predict: "<<endl;
        vector<int> v;
        for(i=0;i<m;i++)
        {
            int x;
            cin>>x;
            v.push_back(x);
        }
        cout<<"Target value: "<<traverse(head, v)<<endl;
        cout<<"Predict another(y/n): ";
        cin>>ch;
    }while(ch == 'y');
}



/*
RAINY   0       HOT     0   HIGH    0   FALSE   0   NO  0
OVERCAST1       MILD    1   NORMAL  1   TRUE    1   YES 1
SUNNY   2       COOL    2


outlook         temp        Humidity    Windy   PlayGolf
14 4
0               0           0           0       0
0               0           0           1       0
1               0           0           0       1
2               1           0           0       1
2               2           1           0       1
2               2           1           1       0
1               2           1           1       1
0               1           0           0       0
0               2           1           0       1
2               1           1           0       1
0               1           1           1       1
1               1           0           1       1
1               0           1           0       1
2               1           0           1       0

*/
