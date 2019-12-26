package com.dmitry.lab6

import com.google.gson.JsonObject
import retrofit2.Call
import retrofit2.http.GET
import retrofit2.http.Query

interface APIService {
    companion object {
        const val API_KEY = "AIzaSyDd5kcyIDuq0H87fJwMyOKA1u7lOt15VYk"
    }

    @GET("v3/playlistItems?part=contentDetails&key=$API_KEY&maxResults=50")
    fun getVideos(@Query("playlistId") listId: String): Call<JsonObject>
}