package com.dmitry.lab6.fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ProgressBar
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.dmitry.lab6.*
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import com.google.gson.JsonObject
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response

class PlaylistFragment(val playlistId: String, val activity: MainActivity) : Fragment() {

    private val retrofit: Retrofit = Retrofit.Builder()
        .baseUrl("https://www.googleapis.com/youtube/")
        .addConverterFactory(GsonConverterFactory.create())
        .build()

    private val service = retrofit.create(APIService::class.java)
    private val videoList = mutableListOf<String>()
    private lateinit var progressBar: ProgressBar

    private fun fetchThumbnails() {
        val thread = Thread {
            val response = service.getVideos(playlistId).execute()
            val items = response.body()!!.getAsJsonArray("items")
            for (item in items) {
                val details = item.asJsonObject.getAsJsonObject("contentDetails")
                val id = details.get("videoId").asString
                videoList.add(id)
            }

            progressBar.visibility = View.GONE
        }

        thread.start()
        thread.join()
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        super.onCreateView(inflater, container, savedInstanceState)

        val rootView = layoutInflater.inflate(R.layout.drawer_playlist, container, false)

        progressBar = rootView.findViewById(R.id.progressBar)
        progressBar.visibility = View.VISIBLE
        fetchThumbnails()

        val recyclerView = rootView.findViewById<RecyclerView>(R.id.playlist_recycler_view)
        recyclerView.apply {
            layoutManager = LinearLayoutManager(activity)
            adapter = ThumbnailAdapter(videoList, activity)
        }

        recyclerView.adapter!!.notifyDataSetChanged()

        activity.setVideo(videoList[0])
        return rootView
    }
}