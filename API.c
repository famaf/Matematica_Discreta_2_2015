#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "API.h"

struct Vertex_Data
{
    u32 id; // Nombre del Vertice
    u32 grado; // Grado del vertice
    u32 color; // Color del vertice
    u32 *vecinos; // Arreglo de vecinos
};

struct GrafoPlus
{
    u32 vertex_count; // Cantidad de vertices del grafo
    u32 edges_count; // Cantidad de lados del grafo
    u32 color_count; // Cantidad de colores del grafo --> "X(G)"
    u32 *array_orden;
    Vertex *vertex_array; // Arreglo de Estructura Vertex
} GrafoSt;


GrafP NuevoGraf()
{
    GrafP G = calloc(1, sizeof(GrafoSt));

    if (G == NULL)
    {
        return NULL;
    }

    G->vertex_count = 0;
    G->edges_count  = 0;
    G->color_count = 0;
    G->vertex_array = NULL;
    G->array_orden = NULL;

    // ASIGNA TODOS LOS CAMPOS DE LA ESTRUCTURA

    return G;
}


int DestruirGraf(GrafP G)
{
    u32 i;

    if(G == NULL)
    {
        return 0;
    }
    
    for(i = 0; i < G->vertex_count; i++)
    {
        free(G->vertex_array[i]->vecinos);
        free(G->vertex_array[i]);
    }

    free(G->vertex_array);

    free(G->array_orden);

    free(G);

    return 1;
    // LIBERA MEMORIA ALOCADA DE LA ESTRUCTURA
}


void SHOW_GRAFO(GrafP G)
{
    u32 j, t;
    printf("-----------------------------------------\n");
    for(j = 0; j < G->vertex_count; j++)
    {
        printf("Vertice: '%u' ==> Grado = %u\n", G->vertex_array[j]->id, G->vertex_array[j]->grado);
        for(t = 0; t < G->vertex_array[j]->grado; t++)
        {
            printf("    Los vecinos de el vertice son '%u': %u\n", G->vertex_array[j]->id, G->vertex_array[G->vertex_array[j]->vecinos[t]]->id);
        }
    }
    printf("-----------------------------------------\n");
}


void SHOW_ORDEN(GrafP G)
{
    u32 i;
    printf("-----------------------------------------\n");
    for(i = 0; i < G->vertex_count; i++)
    {
        printf("Vertice: %u ==> Color: %u ==> Orden: %u ==> Grado: %u\n", G->vertex_array[G->array_orden[i]]->id, G->vertex_array[G->array_orden[i]]->color,G->array_orden[i], G->vertex_array[G->array_orden[i]]->grado);
    }
    printf("-----------------------------------------\n");
}

void ORDEN_EN_LISTA(GrafP G)
{
    u32 i;
    printf("LISTA DE ORDEN DE VERTICES = [  ");
    for (i = 0; i < G->vertex_count; i++)
    {
        printf("%u  ", G->vertex_array[G->array_orden[i]]->id);
    }
    printf("]\n");
}


u32 contador = 0; // Variable Global que me dice hasta cuanto esta lleno el arreglo vertex_array


void add_vertex_id_color_grado(GrafP G, u32 vertex)
{
    bool existe = false;
    u32 i = 0;
    
    while(i < contador && existe == false)
    {
        if (vertex == G->vertex_array[i]->id) // Vertice esta en el array => aumento grado
        {
            G->vertex_array[i]->grado ++;
            existe = true;
        }

        i++;
    }

    if(existe == false) // Si el vertice no esta en el array
    {
        G->vertex_array[i] = calloc(1, sizeof(struct Vertex_Data));
        G->vertex_array[i]->id = vertex;
        G->vertex_array[i]->grado = 1;
        G->vertex_array[i]->color = (i + 1);
        G->array_orden[i] = i;

        contador ++;
    }
}

// Guardamos en vecinos la posición en donde está el vértice vecino->
void add_vecino(GrafP G, u32 vertex_1, u32 vertex_2)
{
    bool existe = false;
    bool find1 = false;
    bool find2 = false;
    u32 i = 0;
    u32 j = 0;
    u32 pos1 = 0;
    u32 pos2 = 0;

    while (i < G->vertex_count && !(find1 && find2))
    {
        if (G->vertex_array[i]->id == vertex_1)
        {
            pos1 = i;
            find1 = true;
        }
        if(G->vertex_array[i]->id == vertex_2)
        {
            pos2 = i;
            find2 = true;
        }
        i++;
    }

    while (j < G->vertex_array[pos1]->grado && !existe)
    {
        if (G->vertex_array[pos1]->vecinos[j] == 0)
        {
            G->vertex_array[pos1]->vecinos[j] = pos2;
            existe = true;
        }
        j++;
    }

    j = 0;
    existe = false;
    
    while (j < G->vertex_array[pos2]->grado && !existe)
    {
        if (G->vertex_array[pos2]->vecinos[j] == 0)
        {
            G->vertex_array[pos2]->vecinos[j] = pos1;
            existe = true;
        }
        j++;
    }
}

//-----------------------------------------------------------------------------
//                          ESTRUCTURA DIMACS (Falta Modificar)
//-----------------------------------------------------------------------------
// static inline char *_non_empty_line(FILE *fd)
// {
//     assert(fd != NULL);
//     char *line = readline(fd);
//     while (strlen(line) == 0)
//     {
//         free(line);
//         line = readline(fd);
//     }

//     return(line);
// }

int LeerGrafo(GrafP G) 
{
    u32 vertex_count = 0, edges_count = 0; // Varibles con vertices y aristas
    u32 scan_result;
    char line; 
    u32 i = 0;
    u32 j;
    u32 k = 0;
    u32 left, right;

// --------------------------PRIMERA RECORRIDA ----------------------------
    while (fscanf(stdin, "%c", &line) != EOF && line != 'p')
    {
        while (fscanf(stdin, "%c", &line) != EOF && line != '\n');
    }

    scan_result = fscanf(stdin, "%*s %u %u\n", &vertex_count, &edges_count);

    if (scan_result != 2)
    {
        return -1;
    }

    G->vertex_count = vertex_count;
    G->edges_count = edges_count;
    G->color_count = vertex_count;

    G->vertex_array = calloc(vertex_count, sizeof(Vertex)); // Tamaño del arreglo de Vertex
    G->array_orden = calloc(vertex_count, sizeof(u32));

    while (i < edges_count && scan_result == 2)
    {
        left = 0;
        right = 0;

        scan_result = fscanf(stdin, "\ne %u %u", &left, &right);

        if (left == right || scan_result != 2)
        {
            return -1;
        }
        else
        {
            add_vertex_id_color_grado(G, left);
            add_vertex_id_color_grado(G, right);
        }

        i++;
    }

    for(j = 0; j < G->vertex_count; j++)
    {
        G->vertex_array[j]->vecinos = calloc(G->vertex_array[j]->grado, sizeof(u32));
    }
// -----------------------------------------------------------------------

    fseek(stdin, 0, 0);

// --------------------------SEGUNDA RECORRIDA ----------------------------
    while (fscanf(stdin, "%c", &line) != EOF && line != 'p')
    {
        while (fscanf(stdin, "%c", &line) != EOF && line != '\n');
    }

    scan_result = fscanf(stdin, "%*s %u %u\n", &vertex_count, &edges_count);

    if (scan_result != 2)
    {
        return -1;
    }

    while (k < edges_count && scan_result == 2)
    {
        left = 0;
        right = 0;

        scan_result = fscanf(stdin, "\ne %u %u", &left, &right);

        add_vecino(G, left, right);

        k++;
    }
// -----------------------------------------------------------------------

    return vertex_count;
}

int ImprimirGrafo(GrafP G)
{
    u32 i;
    u32 k;
    u32 grado_aux = 0;

    printf("p edge %u %u\n", G->vertex_count, G->edges_count);
    
    for(i = 0; i < G->vertex_count; i++)
    {
        grado_aux = G->vertex_array[i]->grado;
        
        for(k = 0; k < grado_aux; k++)
        {
            if(G->vertex_array[i]->vecinos[k] > i)
            {
                printf("e %u %u\n", G->vertex_array[i]->id, G->vertex_array[G->vertex_array[i]->vecinos[k]]->id);
            }
        }
    }

    return 1;
}


u32 CantidadDeColores(GrafP G)
{
    return G->color_count;
}


u32 NumeroVerticesDeColor(GrafP G, u32 i){

    u32 cant_color = 0;
    u32 j;

    for(j = 0; j<G->vertex_count; j++)
    {
        if(G->vertex_array[j]->color == i)
        {
            cant_color ++;
        }
    }
    return cant_color;
}


u32 ImprimirColor(GrafP G, u32 i)
{
    u32 iterator;
    u32 cantidad_vertex_color = 0;
    bool find = false;

    for (iterator = 0; iterator < G->vertex_count; iterator++)
    {
        if (G->vertex_array[iterator]->color == i)
        {
            if (!find)
            {
                printf("Vertices de Color %u: ", i);
                printf("%u", G->vertex_array[iterator]->id);
            }
            else
            {
                printf(", %u", G->vertex_array[iterator]->id);
            }
            
            find = true;
            cantidad_vertex_color ++;
        }
    }

    if (!find)
    {
        printf("No hay vertices de color %u", i);
    }

    printf("->\n");

    return cantidad_vertex_color;
}



u32 Greedy(GrafP G)
{
    u32 i = 0;
    u32 j = 0;
    u32 color_actual = 1;
    u32 grado_aux = 0;
    u32 max_color = 0;

    G->vertex_array[G->array_orden[0]]->color = 1;

    for(i = 1; i < G->vertex_count; i++)
    {
        G->vertex_array[G->array_orden[i]]->color = 0;
    }

    for(i = 1; i < G->vertex_count; i++)
    {
        j = 0;
        color_actual = 1;
        grado_aux = G->vertex_array[G->array_orden[i]]->grado;

        while(j < grado_aux)
        {
            j++;

            if(color_actual == G->vertex_array[G->vertex_array[G->array_orden[i]]->vecinos[j-1]]->color)
            {
                color_actual++;
                j = 0;
            }
        }

        G->vertex_array[G->array_orden[i]]->color = color_actual;

        if(max_color < color_actual)
        {
            max_color = color_actual;
        }
    }

    G->color_count = max_color;

    return max_color;
}

u32 DSATUR(GrafP G){

    u32 i,j,k = 0;
    u32 max_dsatur, dsatur_aux = 0;
    u32 dsatur[G->vertex_count];
    u32 max_grado, grado_aux = 0;
    u32 empate_dsatur, por_colorear = 0;
    u32 color_actual, color_max = 0;
    u32 vecino_j = 0;
    bool falta_colorear = true;
    bool no_actualizar_dsatur = false;

    for(i = 0; i < G->vertex_count; i++)
    {
        dsatur[i] = 0;
        G->vertex_array[G->array_orden[i]]->color = 0;
        
        if(G->vertex_array[G->array_orden[i]]->grado > grado_aux)
        {
            max_grado = i;
            grado_aux = G->vertex_array[G->array_orden[i]]->grado;
        }
    }

    G->vertex_array[G->array_orden[max_grado]]->color = 1;
    
    for(j = 0; j < G->vertex_array[G->array_orden[max_grado]]->grado; j++)
    {
        dsatur[G->vertex_array[G->array_orden[max_grado]]->vecinos[j]]++;
    }

    while(falta_colorear)
    {
        dsatur_aux = 0;
        
        for(i = 0;i < G->vertex_count;i++)
        {
            if(G->vertex_array[G->array_orden[i]]->color == 0)
            {
                if(dsatur[i] > dsatur_aux)
                {
                    max_dsatur = i;
                    dsatur_aux = dsatur[i];
                }
            }
        }

        grado_aux = 0;
        
        for(i = 0; i < G->vertex_count; i++)
        {
            if(G->vertex_array[G->array_orden[i]]->color == 0)
            {
                if(dsatur[i] == dsatur_aux)
                {
                    empate_dsatur++;
                    
                    if(G->vertex_array[G->array_orden[i]]->grado > grado_aux)
                    {
                        grado_aux = G->vertex_array[G->array_orden[i]]->grado;
                        max_grado = i;
                    }
                }
            }
        }

        if(empate_dsatur >= 2)
        {
            por_colorear = max_grado;
        }
        else
        {
            por_colorear = max_dsatur;
        }

        color_actual = 1;
        grado_aux = G->vertex_array[G->array_orden[por_colorear]]->grado;
        j = 0;

        while(j < grado_aux)
        {
            j++;
            
            if(color_actual == G->vertex_array[G->vertex_array[G->array_orden[por_colorear]]->vecinos[j-1]]->color)
            {
                color_actual++;
                j = 0;
            }
        }

        for(j = 0;j < grado_aux; j++)
        {
            vecino_j = G->vertex_array[G->array_orden[por_colorear]]->vecinos[j];
            
            if(G->vertex_array[G->array_orden[vecino_j]]->color == 0)
            {
                k = 0;
                
                while(k < G->vertex_array[G->array_orden[vecino_j]]->grado && no_actualizar_dsatur)
                {
                    i = G->vertex_array[G->vertex_array[G->array_orden[vecino_j]]->vecinos[k]]->color;
                    no_actualizar_dsatur = no_actualizar_dsatur || (color_actual == i);
                    k++;
                }
                
                if(!no_actualizar_dsatur)
                {
                    dsatur[vecino_j]++;
                }
            }
        }

        i = 0;
        falta_colorear = false;
        
        while(i < G->vertex_count && !falta_colorear)
        {
            falta_colorear = falta_colorear || (G->vertex_array[G->array_orden[i]]->color == 0);
            i++;
        }
        
        G->vertex_array[G->array_orden[por_colorear]]->color = color_actual;

        if(color_max < color_actual)
        {
            color_max = color_actual;
        }
    }

    G->color_count = color_max;

    return color_max;
}


void swap(u32 *array, u32 i, u32 j)
{
    u32 tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}


u32 Pivot(u32 *array, GrafP G, u32 left, u32 right)
{
    u32 piv, i, j;
    piv = left;
    i = left + 1;
    j = right;
    
    while (i <= j)
    {
        if (G->vertex_array[(G->array_orden[i])]->grado > G->vertex_array[(G->array_orden[piv])]->grado)
        {
            i++;
        }
        else if (G->vertex_array[(G->array_orden[j])]->grado <= G->vertex_array[(G->array_orden[piv])]->grado)
        {
            j--;
        }
        else
        {
            swap(array, i, j);
            i++;
            j--;
        }
    }

    swap(array, piv, j);
    piv = j;
    
    return piv;
}


void Quick_Sort_Rec(u32 *array, GrafP G, u32 left, u32 right)
{
    u32 piv;
    
    if (right > left && right != 0 && left != UINT32_MAX)
    {
        piv = Pivot(array, G, left, right);
        
        if(piv != 0)
        {
            Quick_Sort_Rec(array, G, left, piv - 1);
        }
        
        if(piv != UINT32_MAX)
        {
            Quick_Sort_Rec(array, G, piv + 1, right);
        }
    }
}


void Quick_Sort(u32 *array, GrafP G, u32 length)
{
    Quick_Sort_Rec(array, G, 0, length - 1);
}


void OrdenWelshPowell(GrafP G)
{
    //SHOW_GRAFO(G);
    // SHOW_ORDEN(G);
    // ORDEN_EN_LISTA(G);

    Quick_Sort(G->array_orden, G, G->vertex_count);

    // ORDEN_EN_LISTA(G); 
    //SHOW_ORDEN(G);
}

/*
    Si 'eleccion' es 'true' => calcula el maximo elemento del arreglo
    Si 'eleccion' es 'false' => calcula el menor elemento del arreglo
*/
u32 Calcular_Mayor(u32 *array, u32 lenght)
{
    u32 mayor = 0;
    u32 posicion;

    for(u32 i = 1; i <= lenght; i++)
    {
        if(array[i] > mayor)
        {
            mayor = array[i];
            posicion = i;
        }
    }

    array[posicion] = 0;

    return posicion;
}

// Se pueden optimizar metiendo el Calcular Maximo adentro y obteniendo la cantidad de vertices con dicho color y disminurlo con cada swap

void GrandeChico(GrafP G)
{
    // SHOW_ORDEN(G);
    // ORDEN_EN_LISTA(G);

    u32 cant_colores = CantidadDeColores(G);

    u32 *array_colores = calloc(cant_colores + 1, sizeof(u32)); // Implicitamente en la posicion 0 del arreglo esta en 0 por calloc

    for(u32 i = 1; i <= cant_colores; i++)
    {
        array_colores[i] = NumeroVerticesDeColor(G, i); // Asigno la cantidad de colores al array de colores
    }
//-----------------------------------------------------------------------------------------------------------------------
    // printf("BEGIN <<< Array de cantidad de vertices por color = [ ");
    // for(u32 i = 1; i <= cant_colores; i++)
    // {
    //     printf("%u ", array_colores[i]);
    // }
    // printf("] >>> END\n");

    // printf("BEGIN <<< Array de cantidad de color por vertices = [ ");
    // for(u32 i = 1; i <= cant_colores; i++)
    // {
    //     printf("%u ", i);
    // }
    // printf("] >>> END\n");
//-----------------------------------------------------------------------------------------------------------------------
    u32 index_color;
    u32 posicion_color = 0;
    u32 j;
    u32 indice_vertice = 0;

    for(index_color = 1; index_color <= cant_colores; index_color++)
    {
        posicion_color = Calcular_Mayor(array_colores, cant_colores); // Calcula el mayor elemento del array de colores y en su posicion pone 0

        j = 0;
        while(j < G->vertex_count && indice_vertice < G->vertex_count)
        {
            if(G->vertex_array[G->array_orden[j]]->color == posicion_color)
            {
                swap(G->array_orden, j, indice_vertice);
                indice_vertice ++;
            }
            j++;
        }
    }

    free(array_colores);

    // ORDEN_EN_LISTA(G);
    // SHOW_ORDEN(G);
}


u32 Calcular_Menor(u32 *array, u32 lenght)
{
    u32 menor = UINT32_MAX;
    u32 posicion;

    for(u32 i = 1; i <= lenght; i++)
    {
        if(array[i] < menor)
        {
            menor = array[i];
            posicion = i;
        }
    }

    array[posicion] = UINT32_MAX;

    return posicion;
}

void ChicoGrande(GrafP G)
{
    // SHOW_ORDEN(G);
    // ORDEN_EN_LISTA(G);

    u32 cant_colores = CantidadDeColores(G);

    u32 *array_colores = calloc(cant_colores + 1, sizeof(u32)); // Implicitamente en la posicion 0 del arreglo esta en 0 por calloc

    for(u32 i = 1; i <= cant_colores; i++)
    {
        array_colores[i] = NumeroVerticesDeColor(G, i); // Asigno la cantidad de colores al array de colores
    }
//-----------------------------------------------------------------------------------------------------------------------
    // printf("BEGIN <<< Array de cantidad de vertices por color = [ ");
    // for(u32 i = 1; i <= cant_colores; i++)
    // {
    //     printf("%u ", array_colores[i]);
    // }
    // printf("] >>> END\n");

    // printf("BEGIN <<< Array de cantidad de color por vertices = [ ");
    // for(u32 i = 1; i <= cant_colores; i++)
    // {
    //     printf("%u ", i);
    // }
    // printf("] >>> END\n");
//-----------------------------------------------------------------------------------------------------------------------
    u32 index_color;
    u32 posicion_color = 0;
    u32 j;
    u32 indice_vertice = 0;

    for(index_color = 1; index_color <= cant_colores; index_color++)
    {
        posicion_color = Calcular_Menor(array_colores, cant_colores); // Calcula el mayor elemento del array de colores y en su posicion pone 0

        j = 0;
        while(j < G->vertex_count && indice_vertice < G->vertex_count)
        {
            if(G->vertex_array[G->array_orden[j]]->color == posicion_color)
            {
                swap(G->array_orden, j, indice_vertice);
                indice_vertice ++;
            }
            j++;
        }
    }

    free(array_colores);

    // ORDEN_EN_LISTA(G);
    // SHOW_ORDEN(G);
}



void Revierte(GrafP G)
{
    // SHOW_ORDEN(G);
    // ORDEN_EN_LISTA(G);
    
    u32 i; // Me indica la posicion en el array de vertices
    u32 index = 0; // Me indica la posicion en el array de orden
    u32 k;

    for (k = CantidadDeColores(G); k > 0; k--) // Recoro los colores del mas grande al mas chixo
    {
        //printf("K = %u\n", k);
        i = 0;
        while(i < G->vertex_count && index < G->vertex_count)
        {
            //printf("WHILE %u\n", i);
            if(G->vertex_array[G->array_orden[i]]->color == k)
            {
                swap(G->array_orden, i, index); // Swapeo las posicion del array de orden
                index++; // Me muevo una posicion mas en el array de orden, las anteriores estarian ordenadas
            }
            i++;
        }
    }

    // ORDEN_EN_LISTA(G);
    // SHOW_ORDEN(G);
}


void OrdenAleatorio(GrafP G)
{
    u32 cantidad_colores = CantidadDeColores(G);
    u32 *array_aleatorio = calloc(cantidad_colores + 1, sizeof(u32)); // [0 ... cantidad_colores] para alamacenar los numeros aleatorios

    bool flag = false; // Para hacer el chequeo de q si el numero aleatorio ya fue elegido
    bool hacer_swap; // Me indica si tengo que hacer o no el swap
    u32 index = 0; // Para recorrer el arreglo de orden
    u32 i = 0; // para recorrer el arreglo de vertices
    u32 numero_aleatorio = 0; // numero aleatorio elegido
    u32 indice_aleatorio = 1; // Indice del arreglo de numeros aleatorios elegidos

    srand(time(NULL)); // Para que los numeros no sean los mismos en cada llamada a la funcion

    numero_aleatorio = rand() % cantidad_colores + 1; // Numero aleatorio entre 1 y cantidad_colores
    printf("Numero Aleatorio: %u\n", numero_aleatorio);

    while (cantidad_colores != 0) // Hacer While mientras no hayan colores
    {
        hacer_swap = true;

// Bloque que corrobora si el numero aleatorio ya fue elegido
        if (flag)
        {
            for(u32 j = 1; j <= cantidad_colores; j++)
            {
                if (numero_aleatorio == array_aleatorio[j])
                {
                    hacer_swap = false; // Si ya fue elegido, no hago el swap y elijo otro numero
                }
            }
        }

        array_aleatorio[indice_aleatorio] = numero_aleatorio; // Guardo el numero aleatorio en el arreglo.

        indice_aleatorio++; // Aumento el indice del arreglo

        flag = true;
//---------------------------- BLOQUE DE SWAP ------------------------------
        if (hacer_swap)
        {
            i = 0;
            while(i < G->vertex_count && index < G->vertex_count)
            {
                if(G->vertex_array[G->array_orden[i]]->color == numero_aleatorio)
                {
                    swap(G->array_orden, i, index); // Swapeo las posicion del array de orden
                    index++; // Me muevo una posicion mas en el array de orden, las anteriores estarian ordenadas
                }
                i++;
            }
            cantidad_colores --; // Disminuyo la cantidad de colores
        }
//-------------------------------------------------------------------------
        numero_aleatorio = rand() % cantidad_colores + 1; // Calculo otro numero aleatorio.
    }

    free(array_aleatorio); // Libero el arreglo de numeros aleatorios.
}