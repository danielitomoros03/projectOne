#include <iostream> 
#include <string>   
#include <stdlib.h> 
#include <cmath>    

using namespace std;

// Enumeraciones
enum NodeType { Harguntino, Enfatico, Motriz, Cambiante };
enum LinkType { Estable, Aporte, Inestable };

// Definición de Estructuras de Datos (Listas Enlazadas)
struct AdjacencyLink {
    int neighbor_node_id;
    int link_id;
    AdjacencyLink* next;
    AdjacencyLink(int n_id, int l_id, AdjacencyLink* n = nullptr) : neighbor_node_id(n_id), link_id(l_id), next(n) {}
};

struct Node {
    char type_char;
    int id;
    NodeType type_enum;
    int maxLinks;
    int used;
    AdjacencyLink* adj_list_head;
    bool visited_flag;
    bool on_recursion_stack_flag;
    int parent_id;
    Node* next;
    Node(char tc, int i, NodeType te, int mL) :
        type_char(tc), id(i), type_enum(te), maxLinks(mL), used(0), adj_list_head(nullptr),
        visited_flag(false), on_recursion_stack_flag(false), parent_id(-1), next(nullptr) {}
};

struct Link {
    int id;
    int node1_id;
    int node2_id;
    LinkType type;
    int energy_contribution;
    int cycle_multiplier;
    bool is_part_of_cycle;
    bool is_part_of_homogeneous_cycle;
    Link* next;
    Link(int i, int n1, int n2, LinkType t, int energy) :
        id(i), node1_id(n1), node2_id(n2), type(t), energy_contribution(energy),
        cycle_multiplier(1), is_part_of_cycle(false), is_part_of_homogeneous_cycle(false), next(nullptr) {}
};

// Nueva estructura para almacenar información de enlaces potenciales
struct PotentialLink {
    int node1_id;
    int node2_id;
    LinkType type;
    int energy;
    PotentialLink* next;
    PotentialLink(int n1, int n2, LinkType t, int e, PotentialLink* n = nullptr) :
        node1_id(n1), node2_id(n2), type(t), energy(e), next(n) {}
};

struct CustomPair {
    long long first;
    int second;
};


long long best_overall_energy = -2e18; // Inicializar con un valor muy bajo
long long best_overall_lifetime = -2e18; // Inicializar con un valor muy bajo
int global_next_node_id = 0;
int global_next_link_id = 0;
Node* global_nodes_head = nullptr;
int global_node_count = 0;

// Variables globales para la mejor estructura encontrada
Node* best_nodes_head = nullptr;
Link* best_links_head = nullptr;

// Variable global para la cabeza de la lista enlazada de enlaces potenciales
PotentialLink* all_potential_links_head = nullptr;

// Función para clonar la lista de nodos y su adyacencia
Node* clone_nodes(Node* orig_head) {
    if (!orig_head) return nullptr;
    Node* new_head = nullptr;
    Node* new_tail = nullptr;
    Node* orig = orig_head;
    while (orig) {
        Node* n = new Node(orig->type_char, orig->id, orig->type_enum, orig->maxLinks);
        n->used = orig->used;
        
        // Clonar adyacencia sin modificar la lista original
        AdjacencyLink* orig_adj_current = orig->adj_list_head;
        AdjacencyLink* new_adj_head = nullptr;
        AdjacencyLink* new_adj_tail = nullptr;
        
        while (orig_adj_current) {
            AdjacencyLink* new_adj = new AdjacencyLink(orig_adj_current->neighbor_node_id, orig_adj_current->link_id);
            if (!new_adj_head) {
                new_adj_head = new_adj;
                new_adj_tail = new_adj;
            } else {
                new_adj_tail->next = new_adj;
                new_adj_tail = new_adj;
            }
            orig_adj_current = orig_adj_current->next;
        }
        n->adj_list_head = new_adj_head;

        if (!new_head) { new_head = n; new_tail = n; }
        else { new_tail->next = n; new_tail = n; }
        orig = orig->next;
    }
    return new_head;
}

// Función para clonar la lista de enlaces
Link* clone_links(Link* orig_head) {
    if (!orig_head) return nullptr;
    Link* new_head = nullptr;
    Link* new_tail = nullptr;
    Link* orig = orig_head;
    while (orig) {
        Link* l = new Link(orig->id, orig->node1_id, orig->node2_id, orig->type, orig->energy_contribution);
        l->cycle_multiplier = orig->cycle_multiplier;
        l->is_part_of_cycle = orig->is_part_of_cycle;
        l->is_part_of_homogeneous_cycle = orig->is_part_of_homogeneous_cycle;
        if (!new_head) { new_head = l; new_tail = l; }
        else { new_tail->next = l; new_tail = l; }
        orig = orig->next;
    }
    return new_head;
}

// Liberar lista de nodos clonados
void free_cloned_nodes(Node* head) {
    Node* n = head;
    while (n) {
        AdjacencyLink* adj = n->adj_list_head;
        while (adj) {
            AdjacencyLink* next = adj->next;
            delete adj;
            adj = next;
        }
        Node* next = n->next;
        delete n;
        n = next;
    }
}

// Liberar lista de enlaces clonados
void free_cloned_links(Link* head) {
    Link* l = head;
    while (l) {
        Link* next = l->next;
        delete l;
        l = next;
    }
}

// Liberar lista de enlaces potenciales
void free_potential_links(PotentialLink* head) {
    PotentialLink* current = head;
    while (current) {
        PotentialLink* next = current->next;
        delete current;
        current = next;
    }
}

// Funciones 
Node* parse_input(const string& input_str);
Node* find_node_by_id(Node* head, int id);
Link* find_link_by_id(Link* head, int id);

// Funciones Auxiliares para Listas Enlazadas
Node* add_node_to_list(Node* head, Node* newNode) {
    if (head == nullptr) return newNode;
    Node* current = head;
    while (current->next != nullptr) current = current->next;
    current->next = newNode;
    return head;
}

Node* find_node_by_id(Node* head, int id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->id == id) return current;
        current = current->next;
    }
    return nullptr;
}

void add_link_to_list(Link*& head, Link* newLink) {
    if (head == nullptr) { head = newLink; return; }
    Link* current = head;
    while (current->next != nullptr) current = current->next;
    current->next = newLink;
}

Link* find_link_by_id(Link* head, int id) {
    Link* current = head;
    while (current != nullptr) {
        if (current->id == id) return current;
        current = current->next;
    }
    return nullptr;
}

Link* remove_last_link_from_list(Link* head) {
    if (head == nullptr) return nullptr;
    if (head->next == nullptr) { // Solo hay un elemento
        delete head;
        return nullptr;
    }
    Link* current = head;
    Link* prev = nullptr;
    while (current->next != nullptr) {
        prev = current;
        current = current->next;
    }
    if (prev != nullptr) {
        prev->next = nullptr;
    }
    delete current; // Liberar el último nodo
    return head;
}

void add_adjacency_link(Node* nodePtr, int neighborId, int linkId) {
    AdjacencyLink* newAdjLink = new AdjacencyLink(neighborId, linkId, nodePtr->adj_list_head);
    nodePtr->adj_list_head = newAdjLink;
    nodePtr->used++; // Incrementar el contador de enlaces usados del nodo
}

void remove_adjacency_link(Node* nodePtr, int neighborId, int linkId) {
    AdjacencyLink* current = nodePtr->adj_list_head;
    AdjacencyLink* prev = nullptr;
    while (current != nullptr && !(current->neighbor_node_id == neighborId && current->link_id == linkId)) {
        prev = current;
        current = current->next;
    }
    if (current != nullptr) { // Enlace encontrado
        if (prev == nullptr) { // Es el primer elemento
            nodePtr->adj_list_head = current->next;
        } else {
            prev->next = current->next;
        }
        delete current; // Liberar la memoria del AdjacencyLink
        nodePtr->used--; // Decrementar el contador de enlaces usados del nodo
    }
}

void delete_nodes_list(Node* head) {
    Node* current_node = head;
    while (current_node != nullptr) {
        Node* next_node = current_node->next;
        AdjacencyLink* adj_link = current_node->adj_list_head;
        while (adj_link != nullptr) {
            AdjacencyLink* next_adj = adj_link->next;
            delete adj_link;
            adj_link = next_adj;
        }
        delete current_node;
        current_node = next_node;
    }
}

void delete_links_list(Link* head) {
    Link* current_link = head;
    while (current_link != nullptr) {
        Link* next_link = current_link->next;
        delete current_link;
        current_link = next_link;
    }
}

bool get_link_properties(const Node& n1, const Node& n2, LinkType& out_type, int& out_energy) {
    // Regla de prioridad: Harguntino (H) forma enlaces estables (0 energía) con todos los otros nodos.
    if (n1.type_enum == Harguntino || n2.type_enum == Harguntino) {
        out_type = Estable;
        out_energy = 0;
        return true;
    }

    // Si ninguno es Harguntino, aplicar las otras reglas
    // Enfatico <-> (cualquiera excepto Harguntino o Cambiante)
    // Como Harguntino ya fue manejado, esto es Enfatico con cualquier nodo que no sea Cambiante.
    if ((n1.type_enum == Enfatico && n2.type_enum != Cambiante) ||
        (n2.type_enum == Enfatico && n1.type_enum != Cambiante)) {
        out_type = Aporte;
        out_energy = 1;
        return true;
    }
    // Motriz <-> (Motriz o Enfatico)
    if ((n1.type_enum == Motriz && (n2.type_enum == Motriz || n2.type_enum == Enfatico)) ||
        (n2.type_enum == Motriz && (n1.type_enum == Motriz || n1.type_enum == Enfatico))) {
        out_type = Aporte;
        out_energy = 1;
        return true;
    }
    // Cambiante <-> (cualquiera excepto Harguntino)
    // Como Harguntino ya fue manejado, esto es Cambiante con cualquier otro nodo.
    if (n1.type_enum == Cambiante || n2.type_enum == Cambiante) {
        out_type = Inestable;
        out_energy = -1;
        return true;
    }
    
    return false; // No se puede formar un enlace con las reglas dadas
}

bool can_form_link(const Node& n1, const Node& n2, LinkType& out_type, int& out_energy) {
    if (n1.used >= n1.maxLinks || n2.used >= n2.maxLinks) {
        return false;
    }
    return get_link_properties(n1, n2, out_type, out_energy);
}

void reset_dfs_flags(Node* nodes_head) {
    Node* current = nodes_head;
    while (current != nullptr) {
        current->visited_flag = false;
        current->on_recursion_stack_flag = false;
        current->parent_id = -1;
        current = current->next;
    }
}

void dfs_for_cycles(Node* u_node, Node* nodes_head, Link* links_head) {
    u_node->visited_flag = true;
    u_node->on_recursion_stack_flag = true;

    AdjacencyLink* adj_link = u_node->adj_list_head;
    while (adj_link != nullptr) {
        Node* v_node = find_node_by_id(nodes_head, adj_link->neighbor_node_id);
        Link* current_link = find_link_by_id(links_head, adj_link->link_id);

        if (v_node == nullptr || current_link == nullptr) {
            adj_link = adj_link->next;
            continue;
        }

        // Evitar el enlace padre en grafos no dirigidos
        if (v_node->id == u_node->parent_id) {
            adj_link = adj_link->next;
            continue;
        }

        if (!v_node->visited_flag) {
            v_node->parent_id = u_node->id;
            dfs_for_cycles(v_node, nodes_head, links_head);
        } else if (v_node->on_recursion_stack_flag) { // Ciclo detectado
            current_link->is_part_of_cycle = true;
            
            bool homogeneous_cycle = true;
            NodeType first_node_type = u_node->type_enum;
            
            Node* temp_node = u_node;
            while (temp_node != nullptr && temp_node->id != v_node->id) {
                if (temp_node->type_enum != first_node_type) {
                    homogeneous_cycle = false;
                }
                temp_node = find_node_by_id(nodes_head, temp_node->parent_id);
            }
            if (temp_node != nullptr && temp_node->type_enum != first_node_type) { // Check v_node itself
                homogeneous_cycle = false;
            }

            temp_node = u_node;
            while (temp_node != nullptr && temp_node->id != v_node->id) {
                Node* parent_node = find_node_by_id(nodes_head, temp_node->parent_id);
                if (parent_node) {
                    AdjacencyLink* temp_adj = temp_node->adj_list_head;
                    while(temp_adj != nullptr) {
                        if (temp_adj->neighbor_node_id == parent_node->id) {
                            Link* cycle_part_link = find_link_by_id(links_head, temp_adj->link_id);
                            if (cycle_part_link != nullptr) {
                                cycle_part_link->is_part_of_cycle = true;
                                if (homogeneous_cycle) {
                                    cycle_part_link->is_part_of_homogeneous_cycle = true;
                                }
                                int multiplier_to_apply = (homogeneous_cycle ? 3 : 2);
                                if (multiplier_to_apply > cycle_part_link->cycle_multiplier) {
                                    cycle_part_link->cycle_multiplier = multiplier_to_apply;
                                }
                            }
                            break;
                        }
                        temp_adj = temp_adj->next;
                    }
                }
                temp_node = parent_node;
            }
            
            if (homogeneous_cycle) {
                current_link->is_part_of_homogeneous_cycle = true;
                if (3 > current_link->cycle_multiplier) {
                    current_link->cycle_multiplier = 3;
                }
            } else {
                if (2 > current_link->cycle_multiplier) {
                    current_link->cycle_multiplier = 2;
                }
            }
        }
        adj_link = adj_link->next;
    }
    u_node->on_recursion_stack_flag = false;
}

long long calculate_total_system_energy(Node* nodes_head, Link* links_head) {
    long long total_energy = 0;
    Link* current_link = links_head;
    while (current_link != nullptr) {
        current_link->is_part_of_cycle = false;
        current_link->is_part_of_homogeneous_cycle = false;
        current_link->cycle_multiplier = 1;
        current_link = current_link->next;
    }

    reset_dfs_flags(nodes_head);
    Node* current_node_dfs_start = nodes_head;
    while (current_node_dfs_start != nullptr) {
        if (!current_node_dfs_start->visited_flag) {
            dfs_for_cycles(current_node_dfs_start, nodes_head, links_head);
        }
        current_node_dfs_start = current_node_dfs_start->next;
    }

    current_link = links_head;
    while (current_link != nullptr) {
        total_energy += (current_link->energy_contribution * current_link->cycle_multiplier);
        current_link = current_link->next;
    }
    
    // Penalización por enlaces no usados (reintroducido y verificado)
    Node* current_node = nodes_head;
    while (current_node != nullptr) {
        total_energy += (current_node->maxLinks - current_node->used) * (-1);
        current_node = current_node->next;
    }
    return total_energy;
}

long long max_path_energy_for_lifetime = -2e18; // Almacena la energía penalizada del mejor camino
int max_path_nodes_for_lifetime = 0;     // Almacena la longitud del mejor camino
bool used_in_path[100]; 

void dfs_longest_simple_path(Node* u_node, Node* nodes_head, Link* links_head,
                          long long current_path_link_energy, int current_path_length,
                          int current_path_node_ids[], int current_path_len_idx) {
    used_in_path[u_node->id] = true;
    current_path_node_ids[current_path_len_idx] = u_node->id;
    current_path_len_idx++;

    bool extended = false;
    AdjacencyLink* adj_link = u_node->adj_list_head;
    while (adj_link != nullptr) {
        Node* v_node = find_node_by_id(nodes_head, adj_link->neighbor_node_id);
        Link* current_link = find_link_by_id(links_head, adj_link->link_id);
        if (v_node != nullptr && !used_in_path[v_node->id] && current_link != nullptr) {
            extended = true;
            dfs_longest_simple_path(v_node, nodes_head, links_head,
                                 current_path_link_energy + (current_link->energy_contribution * current_link->cycle_multiplier),
                                 current_path_length + 1, current_path_node_ids, current_path_len_idx);
        }
        adj_link = adj_link->next;
    }

    if (!extended) { // Fin de un camino
        long long penalized_path_energy = current_path_link_energy;
        for (int i = 0; i < current_path_len_idx; ++i) {
            Node* n = find_node_by_id(nodes_head, current_path_node_ids[i]);
            if (n) {
                penalized_path_energy += (n->maxLinks - n->used) * (-1); // Add penalty for unused slots of nodes IN THE PATH
            }
        }

        if (current_path_length > max_path_nodes_for_lifetime) {
            max_path_nodes_for_lifetime = current_path_length;
            max_path_energy_for_lifetime = penalized_path_energy;
        } else if (current_path_length == max_path_nodes_for_lifetime) {
            if (penalized_path_energy > max_path_energy_for_lifetime) {
                max_path_energy_for_lifetime = penalized_path_energy;
            }
        }
    }
    used_in_path[u_node->id] = false; // Backtrack
}

CustomPair get_longest_path_info(Node* nodes_head, Link* links_head) {
    max_path_energy_for_lifetime = -2e18; // Reset para cada llamada
    max_path_nodes_for_lifetime = 0;

    for (int i = 0; i < 100; ++i) used_in_path[i] = false;

    Node* current_start_node = nodes_head;
    while (current_start_node != nullptr) {
        for (int i = 0; i < 100; ++i) used_in_path[i] = false; 
        int current_path_node_ids[100]; // Array para almacenar los IDs de los nodos en el camino actual
        dfs_longest_simple_path(current_start_node, nodes_head, links_head, 0, 1, current_path_node_ids, 0);
        current_start_node = current_start_node->next;
    }
    CustomPair result;
    result.first = max_path_energy_for_lifetime;
    result.second = max_path_nodes_for_lifetime;
    return result;
}

long long final_best_path_energy = 0; 
int final_best_path_length = 0;     

bool used_in_path_final[100];

void dfs_longest_path_final(Node* u_node, Node* nodes_head, Link* links_head,
                            long long current_path_link_energy, int current_path_length,
                            int current_path_node_ids[], int current_path_len_idx) {
    used_in_path_final[u_node->id] = true;
    current_path_node_ids[current_path_len_idx] = u_node->id;
    current_path_len_idx++;

    bool extended = false;
    AdjacencyLink* adj_link = u_node->adj_list_head;
    while (adj_link != nullptr) {
        Node* v_node = find_node_by_id(nodes_head, adj_link->neighbor_node_id);
        Link* current_link = find_link_by_id(links_head, adj_link->link_id);
        if (v_node != nullptr && !used_in_path_final[v_node->id] && current_link != nullptr) {
            extended = true;
            dfs_longest_path_final(v_node, nodes_head, links_head,
                                   current_path_link_energy + (current_link->energy_contribution * current_link->cycle_multiplier),
                                   current_path_length + 1, current_path_node_ids, current_path_len_idx);
        }
        adj_link = adj_link->next;
    }

    if (!extended) { // Fin de un camino
        long long penalized_path_energy = current_path_link_energy;
        for (int i = 0; i < current_path_len_idx; ++i) {
            Node* n = find_node_by_id(nodes_head, current_path_node_ids[i]);
            if (n) {
                penalized_path_energy += (n->maxLinks - n->used) * (-1);
            }
        }

        // Criterio de selección: mayor longitud, luego mayor energía penalizada
        if (current_path_length > final_best_path_length) {
            final_best_path_length = current_path_length;
            final_best_path_energy = penalized_path_energy;
        } else if (current_path_length == final_best_path_length) {
            if (penalized_path_energy > final_best_path_energy) {
                final_best_path_energy = penalized_path_energy;
            }
        }
    }
    used_in_path_final[u_node->id] = false;
}

void explore_all_structures(PotentialLink* current_p_link, Node* nodes_head, Link*& current_links_head) {
    if (current_p_link == nullptr) {
        long long current_total_energy = calculate_total_system_energy(nodes_head, current_links_head);
        CustomPair longest_path_info = get_longest_path_info(nodes_head, current_links_head);
        long long current_lifetime = abs(current_total_energy) * abs(longest_path_info.first) * longest_path_info.second * 10;

        if (current_total_energy > best_overall_energy) {
            best_overall_energy = current_total_energy;
            best_overall_lifetime = current_lifetime;
            if (best_nodes_head) free_cloned_nodes(best_nodes_head);
            if (best_links_head) free_cloned_links(best_links_head);
            best_nodes_head = clone_nodes(nodes_head);
            best_links_head = clone_links(current_links_head);
        } else if (current_total_energy == best_overall_energy) {
            if (current_lifetime > best_overall_lifetime) {
                best_overall_lifetime = current_lifetime;
                if (best_nodes_head) free_cloned_nodes(best_nodes_head);
                if (best_links_head) free_cloned_links(best_links_head);
                best_nodes_head = clone_nodes(nodes_head);
                best_links_head = clone_links(current_links_head);
            }
        }
        return;
    }

    Node* n1 = find_node_by_id(nodes_head, current_p_link->node1_id);
    Node* n2 = find_node_by_id(nodes_head, current_p_link->node2_id);

    explore_all_structures(current_p_link->next, nodes_head, current_links_head);

    if (n1->used < n1->maxLinks && n2->used < n2->maxLinks) {
        Link* new_link = new Link(global_next_link_id++, n1->id, n2->id, current_p_link->type, current_p_link->energy);
        add_link_to_list(current_links_head, new_link);
        add_adjacency_link(n1, n2->id, new_link->id);
        add_adjacency_link(n2, n1->id, new_link->id);

        explore_all_structures(current_p_link->next, nodes_head, current_links_head);

        remove_adjacency_link(n1, n2->id, new_link->id);
        remove_adjacency_link(n2, n1->id, new_link->id);
        current_links_head = remove_last_link_from_list(current_links_head);
        global_next_link_id--;
    }
}

// Función para precalcular todos los enlaces potenciales y almacenarlos en una lista enlazada
void precompute_potential_links(Node* nodes_head, PotentialLink*& head) {
    // Limpiar la lista existente si la hubiera
    PotentialLink* current = head;
    while (current) {
        PotentialLink* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr; // Asegurarse de que la cabeza esté nula después de limpiar

    Node* current_i = nodes_head;
    while (current_i != nullptr) {
        Node* current_j = current_i->next;
        while (current_j != nullptr) {
            if (current_i->id < current_j->id) { // Asegurar que cada par se considere una sola vez (i,j) y no (j,i)
                LinkType determined_link_type;
                int determined_energy_val;
                if (get_link_properties(*current_i, *current_j, determined_link_type, determined_energy_val)) {
                    // Añadir a la cabeza de la lista de enlaces potenciales
                    PotentialLink* new_p_link = new PotentialLink(current_i->id, current_j->id, determined_link_type, determined_energy_val, head);
                    head = new_p_link;
                }
            }
            current_j = current_j->next;
        }
        current_i = current_i->next;
    }
}

int main() {
    string input_line;
    cin >> input_line;

    global_nodes_head = parse_input(input_line);
    Node* temp_node_ptr = global_nodes_head;
    global_node_count = 0;
    while (temp_node_ptr != nullptr) {
        global_node_count++;
        temp_node_ptr = temp_node_ptr->next;
    }

    precompute_potential_links(global_nodes_head, all_potential_links_head);

    Link* current_structure_links_head = nullptr;
    
    explore_all_structures(all_potential_links_head, global_nodes_head, current_structure_links_head);

    final_best_path_energy = -2e18; // Reset para el cálculo final
    final_best_path_length = 0;
    for (int i = 0; i < 100; ++i) used_in_path_final[i] = false;

    Node* current_start_node = best_nodes_head;
    while (current_start_node != nullptr) {
        for (int i = 0; i < 100; ++i) used_in_path_final[i] = false;
        int current_path_node_ids[100];
        dfs_longest_path_final(current_start_node, best_nodes_head, best_links_head, 0, 1, current_path_node_ids, 0);
        current_start_node = current_start_node->next;
    }

    long long final_lifetime_calculated = abs(best_overall_energy) * abs(final_best_path_energy) * final_best_path_length * 10;

    cout << best_overall_energy << " " << final_lifetime_calculated << endl;

    delete_nodes_list(global_nodes_head);
    if (best_nodes_head) free_cloned_nodes(best_nodes_head);
    if (best_links_head) free_cloned_links(best_links_head);
    free_potential_links(all_potential_links_head); // Liberar la lista de enlaces potenciales

    return 0;
}

Node* parse_input(const string& input_str) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (size_t i = 0; i < input_str.length(); ++i) {
        char current_symbol = input_str[i];
        int count = 1;
        if (i + 1 < input_str.length() && (input_str[i+1] >= '0' && input_str[i+1] <= '9')) {
            count = 0;
            while (i + 1 < input_str.length() && (input_str[i+1] >= '0' && input_str[i+1] <= '9')) {
                count = count * 10 + (input_str[i+1] - '0');
                i++;
            }
        }
        for (int j = 0; j < count; ++j) {
            NodeType node_enum_type;
            int current_max_links = 0;
            if (current_symbol == 'H') {
                node_enum_type = Harguntino;
                current_max_links = 1;
            } else if (current_symbol == 'E') {
                node_enum_type = Enfatico;
                current_max_links = 4;
            } else if (current_symbol == 'M') {
                node_enum_type = Motriz;
                current_max_links = 2;
            } else if (current_symbol == 'C') {
                node_enum_type = Cambiante;
                current_max_links = 2;
            } else {
                continue;
            }
            Node* new_node = new Node(current_symbol, global_next_node_id++, node_enum_type, current_max_links);
            if (head == nullptr) {
                head = new_node;
                tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }
        }
    }
    return head;
}
