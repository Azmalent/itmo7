package com.dmitry.lab6

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import com.dmitry.lab6.fragments.AboutFragment
import com.dmitry.lab6.fragments.HomeFragment
import com.dmitry.lab6.fragments.PlaylistFragment
import com.google.android.youtube.player.YouTubeInitializationResult
import com.google.android.youtube.player.YouTubePlayer
import com.google.android.youtube.player.YouTubePlayerSupportFragment
import com.mikepenz.materialdrawer.AccountHeader
import com.mikepenz.materialdrawer.AccountHeaderBuilder
import com.mikepenz.materialdrawer.Drawer
import com.mikepenz.materialdrawer.DrawerBuilder
import com.mikepenz.materialdrawer.model.DividerDrawerItem
import com.mikepenz.materialdrawer.model.PrimaryDrawerItem
import com.mikepenz.materialdrawer.model.ProfileDrawerItem
import android.view.ViewGroup
import androidx.core.app.ComponentActivity.ExtraData
import androidx.core.content.ContextCompat.getSystemService
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import android.view.View


class MainActivity : AppCompatActivity() {
    companion object {
        const val HOME_ID = 0L
        const val ABOUT_ID = 1L

        const val PLAYLIST_1_ID = 2L
        const val PLAYLIST_2_ID = 3L
        const val PLAYLIST_3_ID = 4L

        val PLAYLISTS = mapOf(
            PLAYLIST_1_ID to "PLWz5rJ2EKKc8I9gHTMh5yKkwRRGE8BjbQ",
            PLAYLIST_2_ID to "PLWz5rJ2EKKc8jQfNAUu5reIGFNNqpn26X",
            PLAYLIST_3_ID to "PLWz5rJ2EKKc-lJo_RGGXL2Psr8vVCTWjM"
        )
    }

    lateinit var drawer: Drawer
    var videoPlayer: YouTubePlayer? = null
    private lateinit var currentVideo: String

    @Suppress("CAST_NEVER_SUCCEEDS")
    fun initPlayerFragment() {
        val parent = supportFragmentManager.findFragmentById(R.id.content)!!
        val child = parent.childFragmentManager.findFragmentById(R.id.youtube_fragment)
        val playerFragment = child as YouTubePlayerSupportFragment

        playerFragment.initialize(APIService.API_KEY, object : YouTubePlayer.OnInitializedListener {
            override fun onInitializationSuccess(provider: YouTubePlayer.Provider, player: YouTubePlayer, wasRestored: Boolean) {
                videoPlayer = player
                player.setPlayerStyle(YouTubePlayer.PlayerStyle.MINIMAL)
                if (!wasRestored) player.cueVideo(currentVideo)
            }

            override fun onInitializationFailure(provider: YouTubePlayer.Provider, error: YouTubeInitializationResult) {

            }
        })
    }

    fun setVideo(id: String) {
        currentVideo = id
        videoPlayer?.cueVideo(currentVideo) ?: initPlayerFragment()
    }

    private fun replaceContent(fragment: Fragment) {
        videoPlayer = null

        val fm = supportFragmentManager
        for(i in 1 until fm.backStackEntryCount) {
            fm.popBackStackImmediate()
        }

        supportFragmentManager.beginTransaction()
            .replace(R.id.content, fragment)
            .addToBackStack("Home")
            .commit()
    }

    private fun displayItem(id: Long) {
        val fragment = when (id) {
            HOME_ID -> HomeFragment()
            ABOUT_ID -> AboutFragment()
            else -> {
                val selectedPlaylist = PLAYLISTS[id] ?: error("drawerItem.identifier")
                PlaylistFragment(selectedPlaylist, this)
            }
        }

        replaceContent(fragment)
    }

    override fun onBackPressed() {
        when {
            drawer.isDrawerOpen -> drawer.closeDrawer()
            drawer.currentSelection == HOME_ID -> for (i in 0 until 3) super.onBackPressed()
            else -> {
                for (i in 0 until 2) supportFragmentManager.popBackStackImmediate()
                drawer.setSelection(1, true)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val header = AccountHeaderBuilder()
            .withActivity(this)
            .addProfiles(
                ProfileDrawerItem().withName("Dmitry").withEmail("dmitry.sjestov@yandex.ru")
            )
            .withOnAccountHeaderListener { _, _, _ -> false }
            .build()

        drawer = DrawerBuilder()
            .withActivity(this)
            .withToolbar(findViewById(R.id.toolbar))
            .withAccountHeader(header)
            .addDrawerItems(
                PrimaryDrawerItem().withIdentifier(HOME_ID).withName(R.string.drawer_home),
                DividerDrawerItem(),
                PrimaryDrawerItem().withIdentifier(PLAYLIST_1_ID).withName(R.string.drawer_playlist1),
                PrimaryDrawerItem().withIdentifier(PLAYLIST_2_ID).withName(R.string.drawer_playlist2),
                PrimaryDrawerItem().withIdentifier(PLAYLIST_3_ID).withName(R.string.drawer_playlist3),
                DividerDrawerItem(),
                PrimaryDrawerItem().withIdentifier(ABOUT_ID).withName(R.string.drawer_about)
            )
            .withOnDrawerItemClickListener { _, _, drawerItem ->
                displayItem(drawerItem.identifier)
                false
            }
            .withCloseOnClick(true)
            .build()
    }
}
