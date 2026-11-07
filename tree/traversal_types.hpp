#pragma once

enum class TraversalOrder {
    KLP, // Корень, Лево, Право (Pre-order)
    KPL, // Корень, Право, Лево
    LKP, // Лево, Корень, Право (In-order)
    PKL, // Право, Корень, Лево
    LPK, // Лево, Право, Корень (Post-order)
    PLK, // Право, Лево, Корень
    BFS  // Поуровневый обход (Breadth-First Search)
};
