#include <iostream> // Para entrada/salida de consola (std::cin, std::cout, std::cerr)
#include <string>   // Para el manejo de cadenas (std::string)
#include <stdlib.h> // Para funciones como abs (std::abs)
#include <cmath>    // Para funciones matemáticas si fueran necesarias, aunque stdlib::abs es suficiente aquí

// Agregado a petición del usuario
using namespace std;

// --- 1. Definición de Enumeraciones ---

// Tipos de nodos básicos
enum NodeType {
    Harguntino, // H
    Enfatico,   // E
    Motriz,     // M
    Cambiante   // C
};

// Tipos de enlaces
enum LinkType {
    Estable,    // 0 energía base
    Aporte,     // +1 energía base
    Inestable   // -1 energía base
};

// --- 2. Definición de Estructuras de Datos (Listas Enlazadas) ---

// a. AdjacencyLink: Para la lista de adyacencia de cada nodo
struct AdjacencyLink {
    int neighbor_node_id; // ID del nodo vecino
    int link_id;          // ID del enlace en la lista global de Link's (para referencia)
    AdjacencyLink* next;  // Puntero al siguiente AdjacencyLink en la lista de adyacencia

    AdjacencyLink(int n_id, int l_id, AdjacencyLink* n = nullptr) :
        neighbor_node_id(n_id), link_id(l_id), next(n) {}
};

// b. Node: Representación de cada nodo en la estructura
struct Node {
    char type_char; // Símbolo del nodo: 'H', 'E', 'M', 'C'
    int id;         // Identificador único del nodo
    NodeType type_enum; // Tipo de nodo como enum para lógica
    int maxLinks;   // Máximo de enlaces que puede formar
    int used;       // Número de enlaces actualmente formados
    AdjacencyLink* adj_list_head; // Cabeza de la lista de adyacencia de este nodo

    // Atributos para DFS (ciclos y camino más largo) - ¡CRUCIAL para evitar arreglos auxiliares!
    bool visited_flag;         // Para marcar si el nodo ha sido visitado en una DFS actual
    bool on_recursion_stack_flag; // Para DFS de ciclo: si está en la pila de recursión actual
    int parent_id;             // Para DFS de ciclo: ID del nodo padre en el árbol DFS actual

    Node* next; // Puntero al siguiente nodo en la lista enlazada global de nodos

    Node(char tc, int i, NodeType te, int mL) :
        type_char(tc), id(i), type_enum(te), maxLinks(mL), used(0), adj_list_head(nullptr),
        visited_flag(false), on_recursion_stack_flag(false), parent_id(-1), next(nullptr) {}
};

// c. Link: Representación de un enlace entre dos nodos
struct Link {
    int id;                 // Identificador único del enlace
    int node1_id;           // ID del primer nodo
    int node2_id;           // ID del segundo nodo
    LinkType type;          // Tipo de enlace (Estable, Aporte, Inestable)
    int energy_contribution; // Energía base del enlace (0, +1, -1)
    int cycle_multiplier;    // Multiplicador de energía por ciclos (1, 2, 3)
    bool is_part_of_cycle;             // Verdadero si es parte de un ciclo
    bool is_part_of_homogeneous_cycle; // Verdadero si es parte de un ciclo homogéneo

    Link* next; // Puntero al siguiente enlace en la lista enlazada global de enlaces de una estructura

    Link(int i, int n1, int n2, LinkType t, int energy) :
        id(i), node1_id(n1), node2_id(n2), type(t), energy_contribution(energy),
        cycle_multiplier(1), is_part_of_cycle(false), is_part_of_homogeneous_cycle(false), next(nullptr) {}
};

// --- 3. Variables Globales ---

// Para la mejor solución encontrada
long long best_overall_energy = -2e18; // Inicializa con un valor muy bajo para asegurar que cualquier energía válida sea mayor
long long best_overall_lifetime = -2e18; // Inicializa con un valor muy bajo

// Contadores de IDs para nuevos nodos y enlaces
int global_next_node_id = 0;
int global_next_link_id = 0;

// Cabeza de la lista global de nodos (la plantilla inicial de nodos)
Node* global_nodes_head = nullptr;
int global_node_count = 0; // Se llenará durante el parseo

// --- Declaraciones de funciones para permitir su uso antes de la definición completa ---
Node* parse_input(const string& input_str); // Declaración anticipada
Node* findNodeById(Node* head, int id); // Declaración anticipada
Link* findLinkById(Link* head, int id); // Declaración anticipada

// --- 4. Funciones Auxiliares para Listas Enlazadas ---

// Añadir un nodo al final de una lista enlazada de Nodos
Node* addNodeToList(Node* head, Node* newNode) {
    if (head == nullptr) {
        return newNode;
    }
    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newNode;
    return head;
}

// Buscar un nodo por su ID en una lista enlazada de Nodos
Node* findNodeById(Node* head, int id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // No encontrado
}

// Añadir un enlace al final de una lista enlazada de Links
// Se pasa head por referencia para poder modificar el puntero si la lista está vacía
void addLinkToList(Link*& head, Link* newLink) { 
    if (head == nullptr) {
        head = newLink;
        return;
    }
    Link* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newLink;
}

// Buscar un enlace por su ID en una lista enlazada de Links
Link* findLinkById(Link* head, int id) {
    Link* current = head;
    while (current != nullptr) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // No encontrado
}

// Remover el último enlace añadido de una lista enlazada de Links (para backtracking)
Link* removeLastLinkFromList(Link* head) {
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

// Añadir un AdjacencyLink a la lista de adyacencia de un nodo (al inicio)
void addAdjacencyLink(Node* nodePtr, int neighborId, int linkId) {
    AdjacencyLink* newAdjLink = new AdjacencyLink(neighborId, linkId, nodePtr->adj_list_head);
    nodePtr->adj_list_head = newAdjLink;
    nodePtr->used++; // Incrementar el contador de enlaces usados del nodo
}

// Remover un AdjacencyLink de la lista de adyacencia de un nodo
void removeAdjacencyLink(Node* nodePtr, int neighborId, int linkId) {
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

// Función para liberar la memoria de una lista enlazada de Nodos y sus AdjacencyLinks
void deleteNodesList(Node* head) {
    Node* current_node = head;
    while (current_node != nullptr) {
        Node* next_node = current_node->next;
        // Liberar la lista de AdjacencyLinks del nodo actual
        AdjacencyLink* adj_link = current_node->adj_list_head;
        while (adj_link != nullptr) {
            AdjacencyLink* next_adj = adj_link->next;
            delete adj_link;
            adj_link = next_adj;
        }
        delete current_node; // Liberar el nodo actual
        current_node = next_node;
    }
}

// Función para liberar la memoria de una lista enlazada de Links (aunque en el backtracking se liberan uno por uno)
void deleteLinksList(Link* head) {
    Link* current_link = head;
    while (current_link != nullptr) {
        Link* next_link = current_link->next;
        delete current_link;
        current_link = next_link;
    }
}

// --- 5. Funciones de Lógica de Negocio ---

// Determina si dos nodos pueden formar un enlace y de qué tipo/energía
bool can_form_and_determine_link_type(const Node& n1, const Node& n2, LinkType& out_type, int& out_energy) {
    // Si los nodos ya están completamente enlazados
    if (n1.used >= n1.maxLinks || n2.used >= n2.maxLinks) {
        return false;
    }

    // Harguntino (H)
    if (n1.type_enum == Harguntino && n2.type_enum == Enfatico) {
        out_type = Aporte;
        out_energy = 1;
        return true;
    }
    if (n2.type_enum == Harguntino && n1.type_enum == Enfatico) {
        out_type = Aporte;
        out_energy = 1;
        return true;
    }

    // Enfático (E)
    if (n1.type_enum == Enfatico && n2.type_enum != Harguntino && n2.type_enum != Cambiante) {
        out_type = Aporte; // Enlaces de aporte con todo nodo no Harguntino, ni de tipo C
        out_energy = 1; // Suministra energía
        return true;
    }
    if (n2.type_enum == Enfatico && n1.type_enum != Harguntino && n1.type_enum != Cambiante) {
        out_type = Aporte; // Enlaces de aporte con todo nodo no Harguntino, ni de tipo C
        out_energy = 1; // Suministra energía
        return true;
    }

    // Motriz (M)
    if (n1.type_enum == Motriz && (n2.type_enum == Motriz || n2.type_enum == Enfatico)) {
        out_type = Aporte; // Enlaces de aporte con nodos de tipo M y de tipo E
        out_energy = 1; // Suministra energía
        return true;
    }
    if (n2.type_enum == Motriz && (n1.type_enum == Motriz || n1.type_enum == Enfatico)) {
        out_type = Aporte; // Enlaces de aporte con nodos de tipo M y de tipo E
        out_energy = 1; // Suministra energía
        return true;
    }

    // Cambiante (C)
    if (n1.type_enum == Cambiante && n2.type_enum != Harguntino) {
        out_type = Inestable; // Enlaces inestables con todo nodo no Harguntino
        out_energy = -1; // Extrae energía
        return true;
    }
    if (n2.type_enum == Cambiante && n1.type_enum != Harguntino) {
        out_type = Inestable; // Enlaces inestables con todo nodo no Harguntino
        out_energy = -1; // Extrae energía
        return true;
    }

    return false; // No se puede formar un enlace compatible
}

// --- DFS para Detección de Ciclos y Aplicación de Multiplicadores ---
// Resetear flags de DFS en todos los nodos antes de iniciar una nueva DFS para ciclos.
void reset_dfs_flags(Node* nodes_head) {
    Node* current = nodes_head;
    while (current != nullptr) {
        current->visited_flag = false;
        current->on_recursion_stack_flag = false;
        current->parent_id = -1;
        current = current->next;
    }
}

// DFS recursivo para encontrar ciclos y marcarlos en los enlaces
void dfs_for_cycles(Node* u_node, Node* nodes_head, Link* links_head) {
    u_node->visited_flag = true;
    u_node->on_recursion_stack_flag = true;

    AdjacencyLink* adj_link = u_node->adj_list_head;
    while (adj_link != nullptr) {
        Node* v_node = findNodeById(nodes_head, adj_link->neighbor_node_id);
        Link* current_link = findLinkById(links_head, adj_link->link_id);

        if (v_node == nullptr || current_link == nullptr) { // Error, nodo/enlace no encontrado
            adj_link = adj_link->next;
            continue;
        }

        // Si el vecino no ha sido visitado
        if (!v_node->visited_flag) {
            v_node->parent_id = u_node->id; // Establecer padre
            dfs_for_cycles(v_node, nodes_head, links_head);
        }
        // Si el vecino ha sido visitado Y está en la pila de recursión (back-edge)
        // Y no es el padre directo (para evitar aristas bidireccionales triviales)
        else if (v_node->on_recursion_stack_flag && v_node->id != u_node->parent_id) {
            // Ciclo detectado
            
            // Marcar el enlace actual como parte de un ciclo
            current_link->is_part_of_cycle = true;

            // Verificar homogeneidad del ciclo y marcar todos los enlaces del ciclo
            bool homogeneous_cycle = true;
            NodeType first_node_type = u_node->type_enum;

            // Recorrer el ciclo para verificar homogeneidad
            Node* current_cycle_node = u_node;
            do {
                if (current_cycle_node->type_enum != first_node_type) {
                    homogeneous_cycle = false;
                }
                // Si el nodo actual no es el origen del back-edge y tiene un padre válido
                if(current_cycle_node->id != v_node->id && current_cycle_node->parent_id != -1) {
                    current_cycle_node = findNodeById(nodes_head, current_cycle_node->parent_id);
                } else {
                    current_cycle_node = nullptr; // Salir del bucle si llegamos al origen del back-edge o un nodo sin padre válido
                }
            } while (current_cycle_node != nullptr && current_cycle_node->id != v_node->id);

            // Marcar todos los enlaces en el ciclo
            current_cycle_node = u_node;
            int prev_node_in_cycle_id = v_node->id; // El nodo que cierra el ciclo con u_node

            // Se recorre desde u_node hacia su padre hasta llegar a v_node (sin incluir v_node)
            // Esto marca todos los enlaces del "camino" del ciclo
            while (current_cycle_node != nullptr && current_cycle_node->id != v_node->id) {
                AdjacencyLink* temp_adj = current_cycle_node->adj_list_head;
                while(temp_adj != nullptr) {
                    if (temp_adj->neighbor_node_id == prev_node_in_cycle_id) {
                        Link* cycle_part_link = findLinkById(links_head, temp_adj->link_id);
                        if (cycle_part_link != nullptr) {
                            cycle_part_link->is_part_of_cycle = true;
                            if (homogeneous_cycle) {
                                cycle_part_link->is_part_of_homogeneous_cycle = true;
                            }
                            // Reemplazo de std::max
                            int multiplier_to_apply = (homogeneous_cycle ? 3 : 2);
                            if (multiplier_to_apply > cycle_part_link->cycle_multiplier) {
                                cycle_part_link->cycle_multiplier = multiplier_to_apply;
                            }
                        }
                        break;
                    }
                    temp_adj = temp_adj->next;
                }
                prev_node_in_cycle_id = current_cycle_node->id;
                current_cycle_node = findNodeById(nodes_head, current_cycle_node->parent_id);
            }
            // El enlace que cierra el ciclo (entre v_node y u_node o su ancestro) ya fue marcado por current_link->is_part_of_cycle = true;
            // Solo asegurarnos de actualizar su multiplicador si es homogéneo
            if (homogeneous_cycle) {
                 current_link->is_part_of_homogeneous_cycle = true;
                 if (3 > current_link->cycle_multiplier) { // Reemplazo de std::max
                     current_link->cycle_multiplier = 3;
                 }
            } else {
                 if (2 > current_link->cycle_multiplier) { // Reemplazo de std::max
                     current_link->cycle_multiplier = 2;
                 }
            }
        }
        adj_link = adj_link->next;
    }

    u_node->on_recursion_stack_flag = false; // Sacar de la pila de recursión al terminar de explorar
}


long long calculate_total_energy(Node* nodes_head, Link* links_head) {
    long long total_energy = 0;

    // 1. Resetear flags de ciclo en enlaces
    Link* current_link = links_head;
    while (current_link != nullptr) {
        current_link->is_part_of_cycle = false;
        current_link->is_part_of_homogeneous_cycle = false;
        current_link->cycle_multiplier = 1; // Resetear a valor base
        current_link = current_link->next;
    }

    // 2. Ejecutar DFS para detección de ciclos
    reset_dfs_flags(nodes_head); // Resetear flags de nodos antes de DFS
    Node* current_node_dfs_start = nodes_head;
    while (current_node_dfs_start != nullptr) {
        if (!current_node_dfs_start->visited_flag) {
            dfs_for_cycles(current_node_dfs_start, nodes_head, links_head);
        }
        current_node_dfs_start = current_node_dfs_start->next;
    }

    // 3. Aplicar multiplicadores de ciclo y sumar energía de enlaces formados
    current_link = links_head;
    while (current_link != nullptr) {
        // Los multiplicadores ya se ajustaron en dfs_for_cycles.
        // Aquí solo los usamos para sumar.
        total_energy += (current_link->energy_contribution * current_link->cycle_multiplier);
        current_link = current_link->next;
    }

    // 4. Sumar energía de enlaces no usados
    /*
    Node* current_node = nodes_head;
    while (current_node != nullptr) {
        total_energy += (current_node->maxLinks - current_node->used) * (-1);
        current_node = current_node->next;
    }
    */

    return total_energy;
}

// Variables globales para el camino más largo que maximiza abs(energía * nodos)
long long max_path_energy_for_lifetime = -2e18; 
int max_path_nodes_for_lifetime = 0; 

// DFS para encontrar el camino más largo
void dfs_for_longest_path(Node* u_node, Node* nodes_head, Link* links_head,
                          long long current_path_energy, int current_path_length) {
    u_node->visited_flag = true;

    // Sumar la penalización de enlaces no usados del nodo actual
    // Esta es la interpretación literal de "menos el peso de los enlaces no usados de los nodos que lo componen"
    // current_path_energy += (long long)(u_node->maxLinks - u_node->used) * (-1);

    // Calcular el valor que se maximiza para el tiempo de vida útil
    long long current_lifetime_product_abs = abs(current_path_energy * (long long)current_path_length);
    long long max_lifetime_product_abs_so_far = abs(max_path_energy_for_lifetime * (long long)max_path_nodes_for_lifetime);


    // Actualizar la mejor solución de camino encontrada hasta ahora
    if (current_lifetime_product_abs > max_lifetime_product_abs_so_far) {
        max_path_energy_for_lifetime = current_path_energy;
        max_path_nodes_for_lifetime = current_path_length;
    } else if (current_lifetime_product_abs == max_lifetime_product_abs_so_far) {
        // En caso de empate en el producto absoluto, priorizar el camino con más nodos
        if (current_path_length > max_path_nodes_for_lifetime) {
            max_path_energy_for_lifetime = current_path_energy;
            max_path_nodes_for_lifetime = current_path_length;
        }
    }

    // Explorar vecinos
    AdjacencyLink* adj_link = u_node->adj_list_head;
    while (adj_link != nullptr) {
        Node* v_node = findNodeById(nodes_head, adj_link->neighbor_node_id);
        Link* current_link = findLinkById(links_head, adj_link->link_id);

        if (v_node != nullptr && !v_node->visited_flag && current_link != nullptr) {
            dfs_for_longest_path(v_node, nodes_head, links_head,
                                 current_path_energy + (current_link->energy_contribution * current_link->cycle_multiplier),
                                 current_path_length + 1);
        }
        adj_link = adj_link->next;
    }
    u_node->visited_flag = false; // Backtrack: desmarcar para explorar otros caminos desde otros puntos de inicio
}

// Función principal para iniciar la búsqueda del camino más largo
pair<long long, int> find_longest_path_info(Node* nodes_head, Link* links_head) {
    max_path_energy_for_lifetime = -2e18; // Reiniciar globales antes de cada búsqueda
    max_path_nodes_for_lifetime = 0;

    Node* current_start_node = nodes_head;
    while (current_start_node != nullptr) {
        reset_dfs_flags(nodes_head); // Resetear flags de nodos para cada inicio de camino
        // La energía inicial es 0 para el camino base, la energía del primer nodo se suma dentro de la DFS
        // La longitud inicial es 1 (el nodo de inicio)
        dfs_for_longest_path(current_start_node, nodes_head, links_head, 0, 1); 
        current_start_node = current_start_node->next;
    }
    return {max_path_energy_for_lifetime, max_path_nodes_for_lifetime};
}


// --- 6. Algoritmo de Backtracking (Generación y Evaluación de Estructuras) ---

void find_best_structure_backtracking(Node* nodes_head, Link*& current_links_head) {
    // 1. Condición de Terminación: Verificar si se pueden añadir más enlaces válidos
    bool can_add_more_links_in_current_state = false;
    Node* i_node = nodes_head;
    while (i_node != nullptr) {
        Node* j_node = i_node->next; // Empezar desde el siguiente para evitar duplicados y auto-enlaces
        while (j_node != nullptr) {
            if (i_node->used < i_node->maxLinks && j_node->used < j_node->maxLinks) {
                // Verificar si el enlace ya existe entre i_node y j_node
                bool link_already_exists = false;
                AdjacencyLink* adj_check = i_node->adj_list_head;
                while (adj_check != nullptr) {
                    if (findNodeById(nodes_head, adj_check->neighbor_node_id) == j_node) {
                        link_already_exists = true;
                        break;
                    }
                    adj_check = adj_check->next;
                }

                if (!link_already_exists) {
                    LinkType temp_type;
                    int temp_energy;
                    if (can_form_and_determine_link_type(*i_node, *j_node, temp_type, temp_energy)) {
                        can_add_more_links_in_current_state = true;
                        break; // Encontramos al menos un posible enlace, podemos seguir
                    }
                }
            }
            j_node = j_node->next;
        }
        if (can_add_more_links_in_current_state) break;
        i_node = i_node->next;
    }

    if (!can_add_more_links_in_current_state) {
        // No se pueden añadir más enlaces: esta es una estructura "completa"
        // Calcular energía y tiempo de vida
        long long current_total_energy = calculate_total_energy(nodes_head, current_links_head);
        pair<long long, int> longest_path_info = find_longest_path_info(nodes_head, current_links_head);

        // CAMBIO AQUÍ: solo multiplicar por nodos del camino más largo
        long long current_lifetime = abs(current_total_energy * longest_path_info.second) * 10;

        // Actualizar la mejor solución global
        if (current_total_energy > best_overall_energy) {
            best_overall_energy = current_total_energy;
            best_overall_lifetime = current_lifetime;
        } else if (current_total_energy == best_overall_energy) {
            if (current_lifetime > best_overall_lifetime) {
                best_overall_lifetime = current_lifetime;
            }
        }
        return; // Termina esta rama de recursión
    }

    // 2. Paso Recursivo: Intentar añadir un enlace
    i_node = nodes_head;
    while (i_node != nullptr) {
        Node* j_node = i_node->next; // Empezar desde el siguiente para evitar duplicados (i-j vs j-i) y auto-enlaces
        while (j_node != nullptr) {
            if (i_node->used < i_node->maxLinks && j_node->used < j_node->maxLinks) {
                // Verificar si el enlace ya existe (otra vez, para evitar recrearlo en la misma rama)
                bool link_already_exists = false;
                AdjacencyLink* adj_check = i_node->adj_list_head;
                while (adj_check != nullptr) {
                    if (findNodeById(nodes_head, adj_check->neighbor_node_id) == j_node) {
                        link_already_exists = true;
                        break;
                    }
                    adj_check = adj_check->next;
                }

                if (!link_already_exists) {
                    LinkType determined_link_type;
                    int determined_energy_val;
                    if (can_form_and_determine_link_type(*i_node, *j_node, determined_link_type, determined_energy_val)) {

                        // --- Hacer el enlace ---
                        Link* new_link = new Link(global_next_link_id++, i_node->id, j_node->id, determined_link_type, determined_energy_val);
                        addLinkToList(current_links_head, new_link); // Llamada corregida

                        addAdjacencyLink(i_node, j_node->id, new_link->id);
                        addAdjacencyLink(j_node, i_node->id, new_link->id);

                        // --- Llamada recursiva ---
                        find_best_structure_backtracking(nodes_head, current_links_head);

                        // --- Deshacer el enlace (Backtrack) ---
                        // Eliminar de las listas de adyacencia
                        removeAdjacencyLink(i_node, j_node->id, new_link->id);
                        removeAdjacencyLink(j_node, i_node->id, new_link->id);

                        // Eliminar de la lista de enlaces y liberar memoria
                        current_links_head = removeLastLinkFromList(current_links_head);
                        global_next_link_id--; // Decrementar ID si se quita el último enlace creado
                    }
                }
            }
            j_node = j_node->next;
        }
        i_node = i_node->next;
    }
}

// --- 7. Función Principal ---

int main() {
    string input_line;
    cin >> input_line;

    // 1. Parsear la entrada y construir la lista enlazada de Nodos
    global_nodes_head = parse_input(input_line);

    // Asegurarse de que global_node_count esté correcto
    Node* temp_node_ptr = global_nodes_head;
    global_node_count = 0;
    while (temp_node_ptr != nullptr) {
        global_node_count++;
        temp_node_ptr = temp_node_ptr->next;
    }

    // 2. Iniciar el algoritmo de backtracking
    Link* current_structure_links_head = nullptr; // La lista de enlaces de la estructura actual
    find_best_structure_backtracking(global_nodes_head, current_structure_links_head);

    // 3. Imprimir el resultado final
    cout << best_overall_energy << " " << best_overall_lifetime << endl;

    // 4. Liberar TODA la memoria asignada dinámicamente
    deleteNodesList(global_nodes_head);
    // current_structure_links_head se maneja dentro del backtracking con removeLastLinkFromList
    // Si la mejor estructura se guardara como una copia de la lista de Links, habría que liberarla aquí.
    // En esta implementación, solo se guardan los valores numéricos de energía y tiempo de vida,
    // y los objetos Link se crean y destruyen en cada rama de backtracking.

    return 0;
}


// --- 8. Implementación de parse_input (reubicada aquí para ser un archivo único) ---
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