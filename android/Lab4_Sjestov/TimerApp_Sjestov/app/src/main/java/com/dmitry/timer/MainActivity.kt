package com.dmitry.timer

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.widget.Button
import android.widget.TextView
import kotlin.math.min

class MainActivity : AppCompatActivity() {

    var isRunning = false
    var elapsedSeconds = 0

    lateinit var startButton: Button
    lateinit var stopButton: Button
    lateinit var resetButton: Button
    lateinit var timer: TextView

    private val handler = Handler()

    lateinit var updateTimer: Runnable
    lateinit var incrementTimer: Runnable

    init {
        updateTimer = Runnable {
            val hours = elapsedSeconds / 3600
            val minutes = (elapsedSeconds % 3600) / 60
            val seconds = elapsedSeconds % 60

            timer.text = "%02d:%02d:%02d".format(hours, minutes, seconds)

            if (isRunning) {
                handler.postDelayed(incrementTimer, 1000)
            } else {
                handler.removeCallbacks(incrementTimer)
            }
        }

        incrementTimer = Runnable {
            elapsedSeconds++
            handler.post(updateTimer)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        savedInstanceState?.let {
            elapsedSeconds = it.getInt("elapsedSeconds")
            isRunning = it.getBoolean("isRunning")
        }

        startButton = findViewById(R.id.start_button)
        stopButton  = findViewById(R.id.stop_button)
        resetButton = findViewById(R.id.reset_button)
        timer       = findViewById(R.id.timer)

        startButton.setOnClickListener {
            isRunning = true
            handler.post(updateTimer)
        }

        stopButton.setOnClickListener {
            isRunning = false
            handler.post(updateTimer)
        }

        resetButton.setOnClickListener {
            isRunning = false
            elapsedSeconds = 0
            handler.post(updateTimer)
        }

        handler.post(updateTimer)
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        
        outState.putInt("elapsedSeconds", elapsedSeconds)
        outState.putBoolean("isRunning", isRunning)
    }
}
