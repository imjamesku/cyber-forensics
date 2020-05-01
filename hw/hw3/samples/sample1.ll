; ModuleID = 'sample1.bc'
source_filename = "sample1.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@g_szTemp = dso_local global [1024 x i8] zeroinitializer, align 16
@.str = private unnamed_addr constant [38 x i8] c"whgt!jtur:01mbnidkovu_tqusee!/o.\22|!uh\00", align 1
@.str.1 = private unnamed_addr constant [23 x i8] c"mw\22ypwr`himg 0few1nvnl\00", align 1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i8* @_Z6encodePc(i8*) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  store i8* %0, i8** %2, align 8
  %4 = load i8*, i8** %2, align 8
  %5 = call i8* @strcpy(i8* getelementptr inbounds ([1024 x i8], [1024 x i8]* @g_szTemp, i64 0, i64 0), i8* %4) #4
  store i32 0, i32* %3, align 4
  br label %6

6:                                                ; preds = %27, %1
  %7 = load i32, i32* %3, align 4
  %8 = icmp slt i32 %7, 200
  br i1 %8, label %9, label %30

9:                                                ; preds = %6
  %10 = load i32, i32* %3, align 4
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds [1024 x i8], [1024 x i8]* @g_szTemp, i64 0, i64 %11
  %13 = load i8, i8* %12, align 1
  %14 = sext i8 %13 to i32
  %15 = icmp ne i32 %14, 0
  br i1 %15, label %16, label %26

16:                                               ; preds = %9
  %17 = load i32, i32* %3, align 4
  %18 = srem i32 %17, 3
  %19 = load i32, i32* %3, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [1024 x i8], [1024 x i8]* @g_szTemp, i64 0, i64 %20
  %22 = load i8, i8* %21, align 1
  %23 = sext i8 %22 to i32
  %24 = add nsw i32 %23, %18
  %25 = trunc i32 %24 to i8
  store i8 %25, i8* %21, align 1
  br label %26

26:                                               ; preds = %16, %9
  br label %27

27:                                               ; preds = %26
  %28 = load i32, i32* %3, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, i32* %3, align 4
  br label %6

30:                                               ; preds = %6
  ret i8* getelementptr inbounds ([1024 x i8], [1024 x i8]* @g_szTemp, i64 0, i64 0)
}

; Function Attrs: nounwind
declare i8* @strcpy(i8*, i8*) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i8* @decode(i8*) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  store i8* %0, i8** %2, align 8
  %4 = load i8*, i8** %2, align 8
  %5 = call i8* @strcpy(i8* getelementptr inbounds ([1024 x i8], [1024 x i8]* @g_szTemp, i64 0, i64 0), i8* %4) #4
  store i32 0, i32* %3, align 4
  br label %6

6:                                                ; preds = %27, %1
  %7 = load i32, i32* %3, align 4
  %8 = icmp slt i32 %7, 200
  br i1 %8, label %9, label %30

9:                                                ; preds = %6
  %10 = load i32, i32* %3, align 4
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds [1024 x i8], [1024 x i8]* @g_szTemp, i64 0, i64 %11
  %13 = load i8, i8* %12, align 1
  %14 = sext i8 %13 to i32
  %15 = icmp ne i32 %14, 0
  br i1 %15, label %16, label %26

16:                                               ; preds = %9
  %17 = load i32, i32* %3, align 4
  %18 = srem i32 %17, 3
  %19 = load i32, i32* %3, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [1024 x i8], [1024 x i8]* @g_szTemp, i64 0, i64 %20
  %22 = load i8, i8* %21, align 1
  %23 = sext i8 %22 to i32
  %24 = sub nsw i32 %23, %18
  %25 = trunc i32 %24 to i8
  store i8 %25, i8* %21, align 1
  br label %26

26:                                               ; preds = %16, %9
  br label %27

27:                                               ; preds = %26
  %28 = load i32, i32* %3, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, i32* %3, align 4
  br label %6

30:                                               ; preds = %6
  ret i8* getelementptr inbounds ([1024 x i8], [1024 x i8]* @g_szTemp, i64 0, i64 0)
}

; Function Attrs: noinline norecurse optnone sspstrong uwtable
define dso_local i32 @main() #2 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %2 = call i8* @decode(i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str, i64 0, i64 0))
  %3 = call i32 @system(i8* %2)
  %4 = call i8* @decode(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i64 0, i64 0))
  %5 = call i32 @system(i8* %4)
  ret i32 0
}

declare i32 @system(i8*) #3

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noinline norecurse optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 9.0.1 "}
