package com.dmitry.lab3.adapters

import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import androidx.recyclerview.widget.RecyclerView
import com.dmitry.lab3.ListElement
import com.squareup.picasso.Picasso
import android.view.LayoutInflater
import android.media.MediaPlayer
import com.dmitry.lab3.MainActivity
import com.dmitry.lab3.R


class ImageAdapter(val images: Array<String>) : RecyclerView.Adapter<ImageAdapter.ImageViewHolder>() {
    class ImageViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val imageView = itemView as ImageView

        fun downloadImage(url: String) {
            Picasso.get().load(url).into(imageView)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ImageViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.image, parent, false)

        val soundPlayer = MediaPlayer.create(parent.context, R.raw.sound)
        view.setOnClickListener {
            soundPlayer.start()
        }

        return ImageViewHolder(view)
    }

    override fun onBindViewHolder(holder: ImageViewHolder, position: Int) {
        val url = images[position]
        holder.downloadImage(url)
    }

    override fun getItemCount(): Int = images.size
}
