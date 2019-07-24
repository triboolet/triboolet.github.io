---
layout: post
title: "La Backpropagation"
date: 2018-11-25 18:15:56 +0100
categories: jekyll update
---

YOO

Bonjour à tous hehe

Ceci est un test : 

<script type="math/tex; mode=display">
\begin{aligned}
\dot{x} & = \sigma(y-x) \\
\dot{y} & = \rho x - y - xz \\
\dot{z} & = -\beta z + xy
\end{aligned}</script>

du coup, ça donne en Python :

{% highlight python %}
def equa(x) :
 for i in range(1,10) :
  x[i,:] += np.array([x[i,j] for j in range(len(x[i]))])
 return x
{% endhighlight %}

Merci à plus
