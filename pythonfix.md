Thanks for confirming. If you’re getting the same requests/JSONDecodeError error even with a fresh Miniconda install, then your environment is still picking up the broken system Python somewhere — even when you use the Miniconda installer with -b -p.

Let’s now completely isolate from system Python, forcibly use only Miniconda’s environment, and verify nothing leaks in from the system. Here’s a hard reset plan that works even on broken systems.

⸻

✅ 100% Isolated Miniconda Fix (Bypass System Python)

🧨 Step 1: Delete Existing Miniconda Install (Just to be sure)

rm -rf ~/miniconda3 ~/.conda ~/.condarc ~/.bashrc-miniconda-backup

Also reset any .bashrc lines that were auto-added by conda init:

# Optional: Open ~/.bashrc and manually remove lines between
# >>> conda initialize >>>
# <<< conda initialize <<<


⸻

✅ Step 2: Fresh Install Without conda init (Manual Activation)

wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
bash miniconda.sh -b -p ~/miniconda3

Then activate Miniconda manually without system Python interference:

export PATH="$HOME/miniconda3/bin:$PATH"

Then test:

which python
python --version
which conda

You should get:

/home/<your-username>/miniconda3/bin/python
Python 3.x.x
/home/<your-username>/miniconda3/bin/conda


⸻

✅ Step 3: Create a Clean Environment

conda create -n cleanenv python=3.10 -y
conda activate cleanenv

You should now be in a pure, isolated Python environment. Test:

python -c "import requests; print(requests.__version__)"

This will use Miniconda’s own requests, not the broken system one.

⸻

🛑 Avoid conda init for Now

Since conda init tries to hook into your shell and can use the system Python to do it, skip it. Instead, manually activate your environments like this in future shells:

export PATH="$HOME/miniconda3/bin:$PATH"
conda activate cleanenv

You can automate that by adding the above two lines to your ~/.bashrc manually instead of running conda init.

⸻

Final Check

Confirm you’re now isolated from system Python:

which python
python --version
python -c "import sys; print(sys.executable)"

Should all point inside ~/miniconda3/envs/cleanenv.

⸻

If this still doesn’t work — please paste the full traceback or terminal output you get when running:

~/miniconda3/bin/python -c "import requests; print(requests.__file__)"

Let’s fix this precisely from there.