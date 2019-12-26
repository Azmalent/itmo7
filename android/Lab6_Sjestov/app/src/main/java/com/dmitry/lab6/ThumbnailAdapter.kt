package com.dmitry.lab6

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import androidx.recyclerview.widget.RecyclerView
import com.squareup.picasso.Picasso

class ThumbnailAdapter(val videoList: List<String>, val activity: MainActivity): RecyclerView.Adapter<RecyclerView.ViewHolder>() {

    inner class ThumbnailViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val thumbnail: ImageView = itemView.findViewById(R.id.thumbnail)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ThumbnailViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.thumbnail, parent, false)
        return ThumbnailViewHolder(view)
    }

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
        val thumbnail = (holder as ThumbnailViewHolder).thumbnail
        val videoId = videoList[position]
        val thumbnailUrl = "http://img.youtube.com/vi/$videoId/default.jpg"
        Picasso.get().load(thumbnailUrl).into(thumbnail)

        thumbnail.setOnClickListener {
            activity.setVideo(videoId)
        }
    }

    override fun getItemCount(): Int {
        return videoList.size
    }
}
