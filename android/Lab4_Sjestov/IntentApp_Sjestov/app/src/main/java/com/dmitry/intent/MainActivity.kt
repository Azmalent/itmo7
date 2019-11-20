package com.dmitry.intent

import android.content.Intent
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import java.nio.file.Files.size
import android.content.pm.ResolveInfo
import android.content.pm.PackageManager
import androidx.core.app.ComponentActivity.ExtraData
import androidx.core.content.ContextCompat.getSystemService
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import android.widget.Toast


class MainActivity : AppCompatActivity() {
    companion object {
        val CALL_URI: Uri   = Uri.parse("tel:${ Uri.encode("*102#") }")
        val MAP_URI: Uri    = Uri.parse("google.streetview:cbll=59.957327,30.308234")
        val WEB_URI: Uri    = Uri.parse("http://www.ifmo.ru")
        val MAIL_URI: Uri   = Uri.parse("mailto:dmitry.sjestov@yandex.ru")
    }

    lateinit var callButton: Button
    lateinit var mapButton: Button
    lateinit var webButton: Button
    lateinit var mailButton: Button

    val callIntent  = Intent(Intent.ACTION_DIAL, CALL_URI)
    val mapIntent   = Intent(Intent.ACTION_VIEW, MAP_URI)
    val webIntent   = Intent(Intent.ACTION_VIEW, WEB_URI)
    val mailIntent  = Intent(Intent.ACTION_SENDTO, MAIL_URI)

    init {
        mailIntent.putExtra(Intent.EXTRA_SUBJECT, "Test email")
        mailIntent.putExtra(Intent.EXTRA_TEXT, "Hello, world!")
    }

    private fun isIntentSafe(intent: Intent): Boolean {
        val activities = packageManager.queryIntentActivities(intent, 0)
        return activities.isNotEmpty()
    }

    private fun tryStartIntent(intent: Intent) {
        if (isIntentSafe(intent)) {
            applicationContext.startActivity(intent)
        } else {
            Toast.makeText(applicationContext, "You have no apps supporting this action!", Toast.LENGTH_LONG).show()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        callButton  = findViewById(R.id.button_call)
        mapButton   = findViewById(R.id.button_map)
        webButton   = findViewById(R.id.button_web)
        mailButton  = findViewById(R.id.button_mail)

        callButton.setOnClickListener {
            tryStartIntent(callIntent)
        }

        mapButton.setOnClickListener {
            tryStartIntent(mapIntent)
        }

        webButton.setOnClickListener {
            val chooser = Intent.createChooser(webIntent, "Open web page...")
            tryStartIntent(chooser)
        }

        mailButton.setOnClickListener {
            val chooser = Intent.createChooser(mailIntent, "Send email...")
            tryStartIntent(chooser)
        }
    }
}
