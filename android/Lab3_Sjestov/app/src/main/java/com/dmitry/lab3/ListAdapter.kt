package com.dmitry.lab3

import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView

class ListAdapter : RecyclerView.Adapter<RecyclerView.ViewHolder>() {
    val list = listOf(
        "https://i.imgur.com/2KxoIsa.jpg",
        "https://i.imgur.com/EZHXbA0.jpg",
        "https://i.imgur.com/qb7Ev7A.jpg",
        "https://i.imgur.com/ZtXXAwX.jpg",
        "https://i.imgur.com/lrE9vha.jpg",
        "https://i.imgur.com/S66rx9n.jpg",
        "https://i.imgur.com/KaSkWef.jpg",
        "https://i.imgur.com/jp5z1s0.jpg",
        "https://i.imgur.com/iLNrl9Z.jpg",
        "https://i.imgur.com/Z3ei74h.jpg",
        "https://i.imgur.com/uXbb81d.jpg",
        "https://i.imgur.com/92mT0BB.jpg"
    ).map { ListElement(it) }

    override fun getItemCount(): Int = list.size

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {

    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {

    }
}